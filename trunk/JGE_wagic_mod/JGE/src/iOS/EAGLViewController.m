#import "EAGLViewController.h"
#import "EAGLView.h"

@interface EAGLViewController (PrivateMethods)
- (NSString*)interfaceOrientationName:(UIInterfaceOrientation) interfaceOrientation;
- (NSString*)deviceOrientationName:(UIDeviceOrientation) deviceOrientation;
@end

@implementation EAGLViewController

@synthesize bannerIsVisible;
@synthesize eaglView;
@synthesize inputField;

#pragma mark initialization / deallocation methods

- (id)init {
    self = [super init];
    if (self) {
        inputField = [[UITextField alloc] initWithFrame: CGRectMake(-50, -50, 25, 25)] ;
        [self.inputField setEnablesReturnKeyAutomatically: YES];
        [self.inputField setEnabled: YES];
        [self.inputField setDelegate: self];
        [inputField setAutoresizingMask:UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight];
        [inputField setAutocapitalizationType:UITextAutocapitalizationTypeNone];
        [inputField setAutocorrectionType:UITextAutocorrectionTypeNo];
        [inputField setKeyboardType: UIKeyboardTypeNamePhonePad];
        CGRect frame = [[UIScreen mainScreen] applicationFrame];
        eaglView = [[EAGLView alloc] initWithFrame:frame];
        [self setView: eaglView];
        [self.view addSubview: inputField];
        [inputField release];
    }
    return self;
}

- (void)dealloc {
    [eaglView setDelegate: nil];
    [eaglView release], eaglView = nil;
    [inputField release], inputField = nil;
    [super dealloc];
}


- (void)viewDidLoad {
	NSLog(@"EAGL ViewController - view Did Load");

	[super viewDidLoad];
}

- (void)viewWillAppear:(BOOL)animated {
	NSLog(@"EAGL ViewController - view Will Appear");
    [self.view resumeGame];	
}


- (void)viewWillDisappear:(BOOL)animated
{
    [self.view pauseGame];
}

- (void)pauseGame
{
    [self.view pauseGame];
}

- (void)resumeGame
{
    [self.view resumeGame];
}

- (void)endGame
{
    [self.view endGame];
}

- (void)viewDidAppear:(BOOL)animated {
	
	NSLog(@"EAGL ViewController - view Did Appear");
	
	UIDeviceOrientation currentDeviceOrientation = [UIDevice currentDevice].orientation;
	UIInterfaceOrientation currentInterfaceOrientation	= self.interfaceOrientation;
	
	NSLog(@"Current Interface: %@. Current Device: %@", 
		  [self interfaceOrientationName:currentInterfaceOrientation], 
		  [self deviceOrientationName:currentDeviceOrientation]);
}



- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

#pragma mark - device orientation handlers

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
    bool isSmallScreen = (UI_USER_INTERFACE_IDIOM()) == UIUserInterfaceIdiomPhone;

    if ( isSmallScreen && UIInterfaceOrientationIsPortrait(interfaceOrientation))
        return NO;
    
    return YES;
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    [[eaglView adView] rotateToOrientation: toInterfaceOrientation];
}


#pragma mark -
#pragma mark Orientation Information
- (NSString*)interfaceOrientationName:(UIInterfaceOrientation) interfaceOrientation {
	
	NSString* result = nil;
	
	switch (interfaceOrientation) {
			
		case UIInterfaceOrientationPortrait:
			result = @"Portrait";
			break;
		case UIInterfaceOrientationPortraitUpsideDown:
			result = @"Portrait UpsideDown";
			break;
		case UIInterfaceOrientationLandscapeLeft:
			result = @"LandscapeLeft";
			break;
		case UIInterfaceOrientationLandscapeRight:
			result = @"LandscapeRight";
			break;
		default:
			result = @"Unknown Interface Orientation";
	}
	
	return result;
};

- (NSString*)deviceOrientationName:(UIDeviceOrientation) deviceOrientation {
	
	NSString* result = nil;
	
	switch (deviceOrientation) {
			
		case UIDeviceOrientationUnknown:
			result = @"Unknown";
			break;
		case UIDeviceOrientationPortrait:
			result = @"Portrait";
			break;
		case UIDeviceOrientationPortraitUpsideDown:
			result = @"Portrait UpsideDown";
			break;
		case UIDeviceOrientationLandscapeLeft:
			result = @"LandscapeLeft";
			break;
		case UIDeviceOrientationLandscapeRight:
			result = @"LandscapeRight";
			break;
		case UIDeviceOrientationFaceUp:
			result = @"FaceUp";
			break;
		case UIDeviceOrientationFaceDown:
			result = @"FaceDown";
			break;
		default:
			result = @"Unknown Device Orientation";
	}
	
	return result;
};

#pragma mark -

#pragma mark UITextFieldDelegate methods
- (void)toggleKeyboardWithState: (NSString *) initialText
{
    UIView *blockerView = [[[UIView alloc] initWithFrame: [self.view frame]] autorelease];
    [blockerView setBackgroundColor: [UIColor clearColor]];
    [self.view addSubview: blockerView];
    if ([[self inputField] becomeFirstResponder])
    {
        [inputField setText: initialText];
        // success in displaying the keyboard
    }
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    if ([string isEqualToString:@" "])
        [eaglView updateKeyboard: @"SPACE"];
    else if ( (string == nil || [string isEqualToString: @""]) && (1 == range.length))
        [eaglView updateKeyboard: @"DELETE"];
    else if ( (string == nil || [string isEqualToString: @""]) && (range.location == (range.length-1)))
        [eaglView updateKeyboard: @"CLEAR"];
    else
        [eaglView updateKeyboard: string];
    
    return YES;
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    return YES;
}


- (BOOL) textFieldShouldReturn:(UITextField *)textField 
{
    [eaglView updateKeyboard: @"SAVE"];
	[textField resignFirstResponder];
	return YES;
}


#pragma mark -
@end
