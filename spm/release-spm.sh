#!/usr/bin/env bash
# Publish a JuggleIM version to the Swift Package distribution repository.
#
# Usage:
#   spm/release-spm.sh <version> <path-to-dist-repo-clone> [--zip <local-zip>] [--push]
#
# Example:
#   spm/release-spm.sh 1.9.3 ../juggleim-ios-spm --zip ~/build/JuggleIM_v1_9_3.zip --push
#
# The zip must already be uploaded to downloads.juggle.im before running with --push.
# Without --push the script only updates and commits locally so you can inspect it.
set -euo pipefail

usage() {
  sed -n '2,11p' "$0" | sed 's/^# \{0,1\}//'
  exit 2
}

[ $# -ge 2 ] || usage
VERSION=$1
DIST_REPO=$2
shift 2

LOCAL_ZIP=""
PUSH=0
while [ $# -gt 0 ]; do
  case "$1" in
    --zip) LOCAL_ZIP=${2:?--zip needs a path}; shift 2 ;;
    --push) PUSH=1; shift ;;
    *) usage ;;
  esac
done

if ! [[ "$VERSION" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
  echo "error: version must be MAJOR.MINOR.PATCH (SPM rejects other forms), got: $VERSION" >&2
  exit 1
fi

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
TEMPLATE_DIR="$SCRIPT_DIR/template"
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)
UNDERSCORED=${VERSION//./_}
URL="https://downloads.juggle.im/libs/ios/JuggleIM_v${UNDERSCORED}.zip"

[ -d "$DIST_REPO/.git" ] || { echo "error: $DIST_REPO is not a git clone" >&2; exit 1; }

if git -C "$DIST_REPO" rev-parse -q --verify "refs/tags/$VERSION" >/dev/null; then
  echo "error: tag $VERSION already exists in $DIST_REPO. Published versions must never change." >&2
  exit 1
fi

WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT
ZIP="$WORK/JuggleIM.zip"

if [ -n "$LOCAL_ZIP" ]; then
  cp "$LOCAL_ZIP" "$ZIP"
else
  echo "Downloading $URL"
  curl -fsSL -o "$ZIP" "$URL"
fi

# SPM requires the zip root to contain exactly the xcframework.
TOP=$(unzip -Z1 "$ZIP" | grep -v '^__MACOSX/' | cut -d/ -f1 | sort -u)
if [ "$TOP" != "JuggleIM.xcframework" ]; then
  echo "error: zip root must contain only JuggleIM.xcframework, found:" >&2
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

sed -e "s#__URL__#$URL#" -e "s#__CHECKSUM__#$CHECKSUM#" \
  "$TEMPLATE_DIR/Package.swift" > "$DIST_REPO/Package.swift"
cp "$TEMPLATE_DIR/README.md" "$DIST_REPO/README.md"
cp "$REPO_ROOT/LICENSE" "$DIST_REPO/LICENSE"

(cd "$DIST_REPO" && swift package dump-package >/dev/null)

git -C "$DIST_REPO" add Package.swift README.md LICENSE
git -C "$DIST_REPO" commit -m "JuggleIM $VERSION"
git -C "$DIST_REPO" tag "$VERSION"

echo "Prepared JuggleIM $VERSION"
echo "  url:      $URL"
echo "  checksum: $CHECKSUM"

if [ "$PUSH" -eq 1 ]; then
  git -C "$DIST_REPO" push origin HEAD
  git -C "$DIST_REPO" push origin "$VERSION"
  echo "Pushed. Users can now resolve $VERSION."
else
  echo "Not pushed. Inspect $DIST_REPO, then run:"
  echo "  git -C $DIST_REPO push origin HEAD && git -C $DIST_REPO push origin $VERSION"
fi
