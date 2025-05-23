#include "desktop_notification.h"

#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>

static desktop_notification::ClickCallback g_click_callback;
static bool g_has_permission = false;
static bool g_tried_initialise = false;

@interface NotificationDelegate : NSObject <UNUserNotificationCenterDelegate>
@end

@implementation NotificationDelegate

- (void)userNotificationCenter:(UNUserNotificationCenter*)center
	didReceiveNotificationResponse:(UNNotificationResponse*)response
			 withCompletionHandler:(void (^)(void))completionHandler {
	if (g_click_callback) {
		g_click_callback();
	}
	completionHandler();
}

- (void)userNotificationCenter:(UNUserNotificationCenter*)center
	   willPresentNotification:(UNNotification*)notification
		 withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler {
	completionHandler(UNNotificationPresentationOptionBanner | UNNotificationPresentationOptionSound);
}

@end

static NotificationDelegate* g_delegate = nil;

namespace desktop_notification {

bool initialise(const std::string& app_name) {
	if (g_tried_initialise)
		return true;

	@autoreleasepool {
		g_delegate = [[NotificationDelegate alloc] init];

		UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
		center.delegate = g_delegate;

		[center getNotificationSettingsWithCompletionHandler:^(UNNotificationSettings* settings) {
		  if (settings.authorizationStatus == UNAuthorizationStatusAuthorized) {
			  g_has_permission = true;
		  }
		  else {
			  [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert | UNAuthorizationOptionSound)
									completionHandler:^(BOOL granted, NSError* error) { g_has_permission = granted; }];
		  }
		}];
	}

	g_tried_initialise = true;
	return true;
}

bool show(const std::string& title, const std::string& message, ClickCallback on_click) {
	if (!g_has_permission) {
		if (!g_tried_initialise) {
			// haven't tried to initialise yet, so do it now
			initialise("doesnt matter - not used");
		}

		return false;
	}

	g_click_callback = on_click;

	@autoreleasepool {
		UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
		content.title = [NSString stringWithUTF8String:title.c_str()];
		content.body = [NSString stringWithUTF8String:message.c_str()];
		content.sound = [UNNotificationSound defaultSound];

		UNNotificationRequest* request = [UNNotificationRequest requestWithIdentifier:[[NSUUID UUID] UUIDString]
																			  content:content
																			  trigger:nil];
		[[UNUserNotificationCenter currentNotificationCenter]
			addNotificationRequest:request
			 withCompletionHandler:^(NSError* _Nullable error) {
			   if (error) {
				   NSLog(@"Failed to deliver notification: %@", error);
			   }
			 }];
	}

	return true;
}

bool is_supported() {
	return true;
}

bool has_permission() {
	return g_has_permission;
}

void cleanup() {
	if (!g_tried_initialise)
		return;

	g_delegate = nil;
	g_tried_initialise = false;
}

} // namespace desktop_notification
