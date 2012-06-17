#import <UIKit/UIKit.h>
#import "WagicDownloadProgressViewController.h"
#import "Reachability.h"

@class EAGLViewController;

@interface wagicAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLViewController *glViewController;
    //Reachability variables
    Reachability  *hostReach, *internetReach, *wifiReach;

}
- (void) rotateBackgroundImage:(UIInterfaceOrientation)fromInterfaceOrientation toInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation;

- (void) handleWEngineCommand:(NSString *) command withParameter: (NSString *) parameter;
- (void) handleWEngineCommand:(NSString *) command withUIParameters: (CGFloat) x yCoordinate: (CGFloat) y width: (CGFloat) width height: (CGFloat) height;

- (void) unpackageResources: (NSString *) folderName pathPrefixName: (NSString *) pathPrefixName;

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) EAGLViewController *glViewController;
@property (nonatomic, retain) WagicDownloadProgressViewController *wagicDownloadController;
@property (nonatomic, retain) Reachability *hostReach, *internetReach, *wifiReach;

@end

