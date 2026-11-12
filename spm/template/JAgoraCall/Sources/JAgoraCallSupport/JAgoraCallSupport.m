// JuggleIM looks the engine up with NSClassFromString, so no code references it
// at link time and the linker would drop it from the static library unless the app
// links with -ObjC. Referencing the class here keeps it linked without -ObjC.
// The class is declared locally because JAgoraCall does not ship JCallMediaAgoraEngine.h in every release.
#import <Foundation/Foundation.h>

@interface JCallMediaAgoraEngine : NSObject
@end

__attribute__((used)) static Class JAgoraCallKeepEngine(void) {
    return [JCallMediaAgoraEngine class];
}
