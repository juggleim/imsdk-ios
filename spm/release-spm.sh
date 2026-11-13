#!/usr/bin/env bash
# Publish JuggleIM or a call plugin to its Swift Package distribution repository.
#
# Usage:
#   spm/release-spm.sh <version> <path-to-dist-repo-clone> [--module <name>]
#                      [--juggleim-min <x.y.z>] [--zip <local-zip>] [--push]
#
# Modules and their distribution repositories (github.com/Juggleim/<repo>):
#   JuggleIM      juggleim-ios-spm      binary package (default module)
#   JZegoCall     jzegocall-ios-spm     binary package
#   JAgoraCall    jagoracall-ios-spm    binary package
#   JLiveKitCall  jlivekitcall-ios-spm  source package, takes no --zip
#
# Examples:
#   spm/release-spm.sh 1.10.2 ../juggleim-ios-spm --zip build/xcframework/JuggleIM_v1_10_2.zip --push
#   spm/release-spm.sh 1.10.1 ../jzegocall-ios-spm --module JZegoCall --juggleim-min 1.10.1 --zip build/xcframework/JZegoCall_v1_10_1.zip --push
#   spm/release-spm.sh 1.10.1 ../jlivekitcall-ios-spm --module JLiveKitCall --juggleim-min 1.10.1 --push
#
# JuggleIM and the plugins are versioned independently. A plugin accepts any
# JuggleIM from --juggleim-min up to (not including) the next major version, so
# JuggleIM can be released alone. Raise --juggleim-min only when the plugin needs
# something new from JuggleIM. JIM.h, JCallMediaEngineProtocol.h and the JCallMedia*
# model headers are the plugins' contract: change them incompatibly only together
# with a major version bump of JuggleIM. --push refuses to publish a plugin until
# JuggleIM <juggleim-min> is published.
# Zips must already be uploaded to downloads.juggle.im before running with --push.
# Without --push the script only updates and commits locally so you can inspect it.
set -euo pipefail

usage() {
  sed -n '2,27p' "$0" | sed 's/^# \{0,1\}//'
  exit 2
}

[ $# -ge 2 ] || usage
VERSION=$1
DIST_REPO=$2
shift 2

MODULE=JuggleIM
JUGGLEIM_MIN=""
LOCAL_ZIP=""
PUSH=0
while [ $# -gt 0 ]; do
  case "$1" in
    --module) MODULE=${2:?--module needs a name}; shift 2 ;;
    --juggleim-min) JUGGLEIM_MIN=${2:?--juggleim-min needs a version}; shift 2 ;;
    --zip) LOCAL_ZIP=${2:?--zip needs a path}; shift 2 ;;
    --push) PUSH=1; shift ;;
    *) usage ;;
  esac
done

SEMVER='^[0-9]+\.[0-9]+\.[0-9]+$'
if ! [[ "$VERSION" =~ $SEMVER ]]; then
  echo "error: version must be MAJOR.MINOR.PATCH (SPM rejects other forms), got: $VERSION" >&2
  exit 1
fi

if [ "$MODULE" = JuggleIM ]; then
  if [ -n "$JUGGLEIM_MIN" ]; then
    echo "error: --juggleim-min is for call plugins, not JuggleIM" >&2
    exit 1
  fi
  JUGGLEIM_NEXT_MAJOR=""
else
  if [ -z "$JUGGLEIM_MIN" ]; then
    echo "error: $MODULE needs --juggleim-min <x.y.z>: the oldest JuggleIM it works with." >&2
    echo "  Keep the value used for the previous $MODULE release unless this release needs newer JuggleIM API." >&2
    exit 1
  fi
  if ! [[ "$JUGGLEIM_MIN" =~ $SEMVER ]]; then
    echo "error: --juggleim-min must be MAJOR.MINOR.PATCH, got: $JUGGLEIM_MIN" >&2
    exit 1
  fi
  # Accept every JuggleIM in the same major version; a major bump marks a break.
  JUGGLEIM_NEXT_MAJOR="$(( ${JUGGLEIM_MIN%%.*} + 1 )).0.0"
fi

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
TEMPLATE_DIR="$SCRIPT_DIR/template/$MODULE"
UNDERSCORED=${VERSION//./_}
CDN="https://downloads.juggle.im/libs/ios"
JUGGLEIM_DIST_URL="https://github.com/Juggleim/juggleim-ios-spm.git"

# Read the version pinned by `s.dependency "NAME", "x.y.z"` in a plugin podspec.
pod_dependency_version() {
  local version
  version=$(sed -nE "s/^ *s\.dependency +[\"']$2[\"'] *, *[\"']([^\"']+)[\"'].*/\1/p" \
    "$REPO_ROOT/$1/$1.podspec")
  [ -n "$version" ] || { echo "error: no $2 version in $1/$1.podspec" >&2; exit 1; }
  echo "$version"
}

case "$MODULE" in
  JuggleIM)
    DIST_NAME=juggleim-ios-spm
    URL="$CDN/JuggleIM_v${UNDERSCORED}.zip"
    DEP_VERSION="" ;;
  JZegoCall)
    DIST_NAME=jzegocall-ios-spm
    URL="$CDN/JZegoCall/JZegoCall_v${UNDERSCORED}.zip"
    DEP_VERSION=$(pod_dependency_version JZegoCall ZegoExpressEngine) ;;
  JAgoraCall)
    DIST_NAME=jagoracall-ios-spm
    URL="$CDN/JAgoraCall/JAgoraCall_v${UNDERSCORED}.zip"
    DEP_VERSION=$(pod_dependency_version JAgoraCall ShengwangRtcEngine_iOS) ;;
  JLiveKitCall)
    # Built from source: the prebuilt JLiveKitCall binary links the CocoaPods module
    # LiveKitClient, but LiveKit's Swift package names the same module LiveKit.
    DIST_NAME=jlivekitcall-ios-spm
    URL=""
    DEP_VERSION=$(pod_dependency_version JLiveKitCall LiveKitClient) ;;
  *)
    echo "error: unknown module $MODULE (expected JuggleIM, JZegoCall, JAgoraCall or JLiveKitCall)" >&2
    exit 1 ;;
esac

if [ -z "$URL" ] && [ -n "$LOCAL_ZIP" ]; then
  echo "error: $MODULE is published as source and takes no --zip" >&2
  exit 1
fi

[ -d "$DIST_REPO/.git" ] || { echo "error: $DIST_REPO is not a git clone" >&2; exit 1; }

# Refuse to write one module into another module's repository.
ORIGIN=$(git -C "$DIST_REPO" remote get-url origin 2>/dev/null || true)
if ! echo "$ORIGIN" | grep -qiE "[/:]Juggleim/$DIST_NAME(\.git)?$"; then
  echo "error: $MODULE belongs in Juggleim/$DIST_NAME, but origin of $DIST_REPO is: ${ORIGIN:-<none>}" >&2
  exit 1
fi

push_release() {
  # Fail instead of prompting for a username when no stored credential works.
  if ! GIT_TERMINAL_PROMPT=0 git -C "$DIST_REPO" push origin HEAD \
    || ! GIT_TERMINAL_PROMPT=0 git -C "$DIST_REPO" push origin "refs/tags/$VERSION"; then
    echo "error: push failed. Fix access (see below) and rerun the same command; the prepared commit and tag are reused." >&2
    echo "  GitHub no longer accepts account passwords. Use an SSH remote or a personal access token:" >&2
    echo "  git -C $DIST_REPO remote set-url origin git@github.com:Juggleim/$DIST_NAME.git" >&2
    exit 1
  fi
  echo "Pushed. Users can now resolve $MODULE $VERSION."
}

# A version is published only once its tag exists on the remote.
if [ "$PUSH" -eq 1 ]; then
  REMOTE_TAGS=$(git -C "$DIST_REPO" ls-remote --tags origin "refs/tags/$VERSION") \
    || { echo "error: cannot reach origin of $DIST_REPO" >&2; exit 1; }
  if [ -n "$REMOTE_TAGS" ]; then
    echo "error: $MODULE $VERSION is already published on origin. Published versions must never change." >&2
    exit 1
  fi
  # A plugin whose minimum JuggleIM is not published cannot be resolved by anyone.
  if [ "$MODULE" != JuggleIM ]; then
    JIM_TAG=$(GIT_TERMINAL_PROMPT=0 git ls-remote --tags "$JUGGLEIM_DIST_URL" "refs/tags/$JUGGLEIM_MIN") \
      || { echo "error: cannot reach $JUGGLEIM_DIST_URL" >&2; exit 1; }
    if [ -z "$JIM_TAG" ]; then
      echo "error: JuggleIM $JUGGLEIM_MIN (--juggleim-min) is not published yet. Release it first:" >&2
      echo "  spm/release-spm.sh $JUGGLEIM_MIN <juggleim-ios-spm clone> --zip build/xcframework/JuggleIM_v${JUGGLEIM_MIN//./_}.zip --push" >&2
      exit 1
    fi
  fi
fi

# A local tag without a remote tag means an earlier push failed: push it, don't rebuild.
if git -C "$DIST_REPO" rev-parse -q --verify "refs/tags/$VERSION" >/dev/null; then
  if [ "$PUSH" -eq 1 ]; then
    echo "Tag $VERSION exists locally but not on origin; retrying push."
    push_release
    exit 0
  fi
  echo "error: tag $VERSION already exists locally. Rerun with --push to publish it, or delete it with:" >&2
  echo "  git -C $DIST_REPO tag -d $VERSION" >&2
  exit 1
fi

WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
CHECKSUM=""

if [ -n "$URL" ]; then
  ZIP="$WORK/$MODULE.zip"
  if [ -n "$LOCAL_ZIP" ]; then
    cp "$LOCAL_ZIP" "$ZIP"
  else
    echo "Downloading $URL"
    curl -fsSL -o "$ZIP" "$URL"
  fi

  # SPM requires the zip root to contain exactly the xcframework.
  TOP=$(unzip -Z1 "$ZIP" | grep -v '^__MACOSX/' | cut -d/ -f1 | sort -u)
  if [ "$TOP" != "$MODULE.xcframework" ]; then
    echo "error: zip root must contain only $MODULE.xcframework, found:" >&2
    echo "$TOP" >&2
    exit 1
  fi

  CHECKSUM=$(swift package compute-checksum "$ZIP")

  if [ -n "$LOCAL_ZIP" ] && [ "$PUSH" -eq 1 ]; then
    # The local file must be byte-identical to what users will download.
    curl -fsSL -o "$WORK/remote.zip" "$URL" || { echo "error: $URL is not reachable; upload the zip first" >&2; exit 1; }
    REMOTE=$(swift package compute-checksum "$WORK/remote.zip")
    if [ "$REMOTE" != "$CHECKSUM" ]; then
      echo "error: uploaded zip differs from --zip file ($REMOTE != $CHECKSUM)" >&2
      exit 1
    fi
  fi
fi

fill_template() {
  sed -e "s#__URL__#$URL#g" \
      -e "s#__CHECKSUM__#$CHECKSUM#g" \
      -e "s#__VERSION__#$VERSION#g" \
      -e "s#__DEP_VERSION__#$DEP_VERSION#g" \
      -e "s#__JUGGLEIM_MIN__#$JUGGLEIM_MIN#g" \
      -e "s#__JUGGLEIM_NEXT_MAJOR__#$JUGGLEIM_NEXT_MAJOR#g" \
      "$1" > "$2"
  if grep -q '__[A-Z_]*__' "$2"; then
    echo "error: unfilled placeholder left in $2" >&2
    exit 1
  fi
}

# Sources/ is regenerated on every release, so files dropped from a module go away too.
rm -rf "$DIST_REPO/Sources"
fill_template "$TEMPLATE_DIR/Package.swift" "$DIST_REPO/Package.swift"
fill_template "$TEMPLATE_DIR/README.md" "$DIST_REPO/README.md"
cp "$REPO_ROOT/LICENSE" "$DIST_REPO/LICENSE"
if [ -d "$TEMPLATE_DIR/Sources" ]; then
  cp -R "$TEMPLATE_DIR/Sources" "$DIST_REPO/Sources"
fi

if [ "$MODULE" = JLiveKitCall ]; then
  SRC="$REPO_ROOT/JLiveKitCall/JLiveKitCall"
  DEST="$DIST_REPO/Sources/JLiveKitCall"
  mkdir -p "$DEST/include"
  cp "$SRC/JCallMediaLiveKitEngine.h" "$DEST/include/"
  cp "$SRC/JCallMediaLiveKitEngine.m" "$DEST/"
  COUNT=$(grep -c '^@import LiveKitClient;$' "$DEST/JCallMediaLiveKitEngine.m" || true)
  if [ "$COUNT" != 1 ]; then
    echo "error: expected one '@import LiveKitClient;' in $SRC/JCallMediaLiveKitEngine.m, found $COUNT" >&2
    exit 1
  fi
  perl -pi -e 's/^\@import LiveKitClient;$/\@import LiveKit;/' "$DEST/JCallMediaLiveKitEngine.m"
fi

(cd "$DIST_REPO" && swift package dump-package >/dev/null)

git -C "$DIST_REPO" add Package.swift README.md LICENSE
if [ -e "$DIST_REPO/Sources" ] || [ -n "$(git -C "$DIST_REPO" ls-files Sources)" ]; then
  git -C "$DIST_REPO" add -A Sources
fi
git -C "$DIST_REPO" commit -m "$MODULE $VERSION"
git -C "$DIST_REPO" tag "$VERSION"

echo "Prepared $MODULE $VERSION"
if [ "$MODULE" != JuggleIM ]; then
  echo "  juggleim: $JUGGLEIM_MIN..<$JUGGLEIM_NEXT_MAJOR"
fi
if [ -n "$URL" ]; then
  echo "  url:      $URL"
  echo "  checksum: $CHECKSUM"
fi

if [ "$PUSH" -eq 1 ]; then
  push_release
else
  echo "Not pushed. Inspect $DIST_REPO, then rerun the same command with --push."
fi
