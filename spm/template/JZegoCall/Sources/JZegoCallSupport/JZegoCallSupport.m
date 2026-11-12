// JuggleIM looks the engine up with NSClassFromString, so no code references it
// at link time and the linker would drop it from the static library unless the app
// links with -ObjC. Referencing the class here keeps it linked without -ObjC.
// The class is declared locally because JZegoCall does not ship JCallMediaZegoEngine.h in every release.
#import <Foundation/Foundation.h>

@interface JCallMediaZegoEngine : NSObject
@end

__attribute__((used)) static Class JZegoCallKeepEngine(void) {
    return [JCallMediaZegoEngine class];
}
