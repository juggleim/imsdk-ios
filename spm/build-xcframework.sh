#!/usr/bin/env bash
# Build JuggleIM and the call plugins, zip them, and sync every version number.
#
# Usage:
#   spm/build-xcframework.sh <version>
#
# Example:
#   spm/build-xcframework.sh 1.9.4
#
# Steps:
#   1. Write <version> into every place that carries the SDK version:
#        JuggleIM/JuggleIM/internal/JuggleIMConstInternal.h   JIMVersion
#        QuickStart/QuickStart.xcodeproj/project.pbxproj      MARKETING_VERSION
#        QuickStart/Sources/JuggleUI.swift                    JuggleUI.version
#        JuggleIM, JZegoCall, JAgoraCall, JLiveKitCall podspecs  s.version
#   2. Build JuggleIM, then build each plugin against that fresh JuggleIM.
#   3. Zip each xcframework into build/xcframework/ and write its URL and
#      SHA-256 into the matching podspec's s.source.
#
# Upload the zips to the printed URLs, then publish SPM (JuggleIM first) with
# spm/release-spm.sh; run it without arguments for the per-module commands.
#
# Needs network access: JLiveKitCall is built through CocoaPods (LiveKitClient).
# The xcframeworks are intentionally NOT code signed, matching all earlier releases.
# Debug info is kept (STRIP_INSTALLED_PRODUCT=NO) so apps that link these static
# libraries get JuggleIM symbols in their own dSYM and can symbolicate crashes.
set -euo pipefail

usage() {
  sed -n '2,26p' "$0" | sed 's/^# \{0,1\}//'
  exit 2
}

[ $# -eq 1 ] || usage
VERSION=$1

if ! [[ "$VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "error: version must be MAJOR.MINOR.PATCH, got: $VERSION" >&2
  exit 1
fi

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
OUT="$REPO_ROOT/build/xcframework"
WORK="$REPO_ROOT/build/work"
VERSION_US=${VERSION//./_}
CDN="https://downloads.juggle.im/libs/ios"
PLUGINS=(JZegoCall JAgoraCall JLiveKitCall)

# Replace every line matching PATTERN in FILE with REPLACEMENT, and fail unless
# exactly EXPECTED lines matched. PATTERN must be valid for both grep -E and perl.
set_line() {
  local file=$1 pattern=$2 replacement=$3 expected=$4 count
  count=$(grep -cE "$pattern" "$file" || true)
  if [ "$count" != "$expected" ]; then
    echo "error: expected $expected line(s) matching /$pattern/ in $file, found $count" >&2
    exit 1
  fi
  PAT="$pattern" REP="$replacement" perl -pi -e 's/$ENV{PAT}/$ENV{REP}/' "$file"
}

podspec_of() { echo "$REPO_ROOT/$1/$1.podspec"; }

# Read `s.ios.deployment_target = 'x.y'` from a podspec.
deployment_target_of() {
  sed -nE "s/^ *s\.ios\.deployment_target *= *'([^']+)'.*/\1/p" "$(podspec_of "$1")"
}

# --- 1. Version numbers -------------------------------------------------------

echo "Setting version $VERSION"
set_line "$REPO_ROOT/JuggleIM/JuggleIM/internal/JuggleIMConstInternal.h" \
  '^#define JIMVersion @"[^"]*"' "#define JIMVersion @\"$VERSION\"" 1
set_line "$REPO_ROOT/QuickStart/QuickStart.xcodeproj/project.pbxproj" \
  'MARKETING_VERSION = [^;]*;' "MARKETING_VERSION = $VERSION;" 2
set_line "$REPO_ROOT/QuickStart/Sources/JuggleUI.swift" \
  'public static let version: String = "[^"]*"' "public static let version: String = \"$VERSION\"" 1
for pod in JuggleIM "${PLUGINS[@]}"; do
  set_line "$(podspec_of "$pod")" "^  s\.version +=.*$" "  s.version          = '$VERSION'" 1
done

rm -rf "$OUT" "$WORK"
mkdir -p "$OUT" "$WORK"

# Merge a device and a simulator archive of NAME into OUT/NAME.xcframework.
create_xcframework() {
  local name=$1 device=$2 simulator=$3
  xcodebuild -create-xcframework \
    -archive "$device" -framework "$name.framework" \
    -archive "$simulator" -framework "$name.framework" \
    -output "$OUT/$name.xcframework"
}

# Zip OUT/NAME.xcframework, point NAME's podspec at URL with the zip's SHA-256.
zip_and_publish_source() {
  local name=$1 url=$2 zip="$OUT/$(basename "$2")" sha
  # Use zip, not `ditto -c -k`: ditto stores macOS metadata as ._* AppleDouble
  # entries, which unzip as stray files inside the framework.
  # -y keeps symlinks; -X drops extra file attributes.
  (cd "$OUT" && zip -q -r -y -X "$zip" "$name.xcframework")
  if unzip -Z1 "$zip" | grep -qE '(^|/)\._|^__MACOSX/'; then
    echo "error: $zip contains AppleDouble ._* entries" >&2
    exit 1
  fi
  # SwiftPM's checksum is the zip's SHA-256, so CocoaPods and SPM share one value.
  sha=$(shasum -a 256 "$zip" | awk '{print $1}')
  set_line "$(podspec_of "$name")" '^  s\.source +=.*$' \
    "  s.source           = { :http => '$url', :sha256 => '$sha' }" 1
  echo "$name: $url"
  echo "  sha256: $sha"
}

# --- 2. JuggleIM --------------------------------------------------------------

archive_juggleim() {
  xcodebuild archive \
    -project "$REPO_ROOT/JuggleIM/JuggleIM.xcodeproj" \
    -scheme JuggleIM \
    -configuration Release \
    -destination "$1" \
    -archivePath "$2" \
    -derivedDataPath "$WORK/dd-JuggleIM" \
    SKIP_INSTALL=NO \
    STRIP_INSTALLED_PRODUCT=NO \
    BUILD_LIBRARY_FOR_DISTRIBUTION=YES \
    CODE_SIGNING_ALLOWED=NO \
    -quiet
}

echo "Building JuggleIM"
archive_juggleim "generic/platform=iOS" "$WORK/JuggleIM-ios.xcarchive"
archive_juggleim "generic/platform=iOS Simulator" "$WORK/JuggleIM-sim.xcarchive"
create_xcframework JuggleIM "$WORK/JuggleIM-ios.xcarchive" "$WORK/JuggleIM-sim.xcarchive"

# Every slice must ship the privacy manifest; App Store Connect checks for it.
for slice in "$OUT"/JuggleIM.xcframework/*/JuggleIM.framework; do
  if [ ! -f "$slice/PrivacyInfo.xcprivacy" ]; then
    echo "error: $slice is missing PrivacyInfo.xcprivacy" >&2
    exit 1
  fi
  plutil -lint -s "$slice/PrivacyInfo.xcprivacy"
done

JIM_XCF="$OUT/JuggleIM.xcframework"
JIM_DEVICE_SLICE=$(ls -d "$JIM_XCF"/ios-arm64)
JIM_SIM_SLICE=$(ls -d "$JIM_XCF"/ios-*-simulator)

# --- 3. Call plugins ----------------------------------------------------------

# Build in a copy so CocoaPods integration and Xcode state never touch the repo.
copy_plugin() {
  rsync -a --exclude DerivedData --exclude build --exclude xcuserdata \
    "$REPO_ROOT/$1/" "$WORK/$1/"
}

# JZegoCall and JAgoraCall link a vendored RTC xcframework from their framework/
# folder and import JuggleIM from the build products, so point both the compiler
# and the module verifier at the matching slice of the JuggleIM just built.
# The verifier only forwards clang flags that come after a `--` separator.
archive_vendored_plugin() {
  local name=$1 destination=$2 slice=$3 path=$4
  xcodebuild archive \
    -project "$WORK/$name/$name.xcodeproj" \
    -scheme "$name" \
    -configuration Release \
    -destination "$destination" \
    -archivePath "$path" \
    -derivedDataPath "$WORK/dd-$name" \
    SKIP_INSTALL=NO \
    STRIP_INSTALLED_PRODUCT=NO \
    CODE_SIGNING_ALLOWED=NO \
    IPHONEOS_DEPLOYMENT_TARGET="$(deployment_target_of "$name")" \
    "FRAMEWORK_SEARCH_PATHS=\$(inherited) $slice" \
    "OTHER_MODULE_VERIFIER_FLAGS=\$(inherited) -- -F$slice" \
    -quiet
}

for name in JZegoCall JAgoraCall; do
  echo "Building $name"
  copy_plugin "$name"
  archive_vendored_plugin "$name" "generic/platform=iOS" "$JIM_DEVICE_SLICE" "$WORK/$name-ios.xcarchive"
  archive_vendored_plugin "$name" "generic/platform=iOS Simulator" "$JIM_SIM_SLICE" "$WORK/$name-sim.xcarchive"
  create_xcframework "$name" "$WORK/$name-ios.xcarchive" "$WORK/$name-sim.xcarchive"
done

# JLiveKitCall imports LiveKitClient, which only ships through CocoaPods. Build it
# in a Pods workspace that uses the JuggleIM just built and the LiveKitClient
# version pinned in JLiveKitCall.podspec.
echo "Building JLiveKitCall"
copy_plugin JLiveKitCall
LIVEKIT_VERSION=$(sed -nE 's/.*dependency +"LiveKitClient", *"([^"]+)".*/\1/p' "$(podspec_of JLiveKitCall)")
[ -n "$LIVEKIT_VERSION" ] || { echo "error: no LiveKitClient version in JLiveKitCall.podspec" >&2; exit 1; }
mkdir -p "$WORK/local-juggleim"
cp -R "$JIM_XCF" "$WORK/local-juggleim/"
cat > "$WORK/local-juggleim/JuggleIM.podspec" <<EOF
Pod::Spec.new do |s|
  s.name = 'JuggleIM'
  s.version = '$VERSION'
  s.summary = 'Locally built JuggleIM for packaging the call plugins.'
  s.homepage = 'https://juggle.im/'
  s.license = { :type => 'Copyright', :text => 'Copyright Juggle' }
  s.author = { 'Juggle' => 'support@juggle.im' }
  s.source = { :path => '.' }
  s.ios.deployment_target = '12.0'
  s.vendored_frameworks = 'JuggleIM.xcframework'
end
EOF
cat > "$WORK/JLiveKitCall/Podfile" <<EOF
source 'https://cdn.cocoapods.org/'
source 'https://github.com/livekit/podspecs.git'
platform :ios, '$(deployment_target_of JLiveKitCall)'
project 'JLiveKitCall.xcodeproj'
target 'JLiveKitCall' do
  pod 'JuggleIM', :path => '$WORK/local-juggleim'
  pod 'LiveKitClient', '$LIVEKIT_VERSION'
end
EOF
(cd "$WORK/JLiveKitCall" && LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 pod install)

# No BUILD_LIBRARY_FOR_DISTRIBUTION here: it would also apply to the LiveKitClient
# pod, whose generated module interface fails to compile with it.
# Script sandboxing is off because the CocoaPods resources script writes into Pods/.
archive_livekit() {
  xcodebuild archive \
    -workspace "$WORK/JLiveKitCall/JLiveKitCall.xcworkspace" \
    -scheme JLiveKitCall \
    -configuration Release \
    -destination "$1" \
    -archivePath "$2" \
    -derivedDataPath "$WORK/dd-JLiveKitCall" \
    SKIP_INSTALL=NO \
    STRIP_INSTALLED_PRODUCT=NO \
    CODE_SIGNING_ALLOWED=NO \
    ENABLE_USER_SCRIPT_SANDBOXING=NO \
    IPHONEOS_DEPLOYMENT_TARGET="$(deployment_target_of JLiveKitCall)" \
    -quiet
}
archive_livekit "generic/platform=iOS" "$WORK/JLiveKitCall-ios.xcarchive"
archive_livekit "generic/platform=iOS Simulator" "$WORK/JLiveKitCall-sim.xcarchive"
create_xcframework JLiveKitCall "$WORK/JLiveKitCall-ios.xcarchive" "$WORK/JLiveKitCall-sim.xcarchive"

# --- 4. Zip and record checksums ----------------------------------------------

echo
zip_and_publish_source JuggleIM "$CDN/JuggleIM_v$VERSION_US.zip"
for name in "${PLUGINS[@]}"; do
  zip_and_publish_source "$name" "$CDN/$name/${name}_v$VERSION_US.zip"
done

rm -rf "$WORK"
echo
echo "Zips are in $OUT. Upload each one to the URL printed above."
