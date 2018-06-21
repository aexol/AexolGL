#ifndef FB_PERMISSIONS_AEX_HEADER_FILE
#define FB_PERMISSIONS_AEX_HEADER_FILE

#include <string>
#include <vector>

namespace aex
{
    namespace fb
    {
        enum class PERMISSION
        {
            NONE,
            PUBLIC_PROFILE,
            USER_FRIENDS,
            EMAIL,
            USER_ABOUT_ME,
            USER_ACTIONS_BOOKS,
            USER_ACTIONS_FITNESS,
            USER_ACTIONS_MUSIC,
            USER_ACTIONS_NEWS,
            USER_ACTIONS_VIDEO,
            USER_BIRTHDAY,
            USER_EDUCATION_HISTORY,
            USER_EVENTS,
            USER_GAMES_ACTIVITY,
            USER_HOMETOWN,
            USER_LIKES,
            USER_LOCATION,
            USER_MANAGED_GROUPS,
            USER_PHOTOS,
            USER_POSTS,
            USER_RELATIONSHIPS,
            USER_RELATIONSHIP_DETAILS,
            USER_RELIGION_POLITICS,
            USER_TAGGED_PLACES,
            USER_VIDEOS,
            USER_WEBSITE,
            USER_WORK_HISTORY,
            READ_CUSTOM_FRIENDLISTS,
            READ_INSIGHTS,
            READ_AUDIENCE_NETWORK_INSIGHTS,
            READ_PAGE_MAILBOXES,
            MANAGE_PAGES,
            PUBLISH_PAGES,
            PUBLISH_ACTIONS,
            RSVP_EVENT,
            PAGES_SHOW_LIST,
            PAGES_MANAGE_CTA,
            PAGES_MANAGE_INSTANT_ARTICLES,
            ADS_READ,
            ADS_MANAGEMENT,
            BUSINESS_MANAGEMENT,
            PAGES_MESSAGING,
            PAGES_MESSAGING_SUBSCRIPTIONS,
            PAGES_MESSAGING_PAYMENTS,
            PAGES_MESSAGING_PHONE_NUMBER
        };

        std::string permissionToString(PERMISSION perm);
        PERMISSION stringToPermission(const std::string& permStr);

        class FBPermissionsGroup
        {
        public:
            FBPermissionsGroup();
            virtual ~FBPermissionsGroup();
            void addPermission(PERMISSION perm);
        protected:
            std::string getPermissionsJoined() const;
            std::vector<std::string> getPermissionsAsStrings() const;
            std::vector<PERMISSION> m_permissions;
            static std::vector<std::string> splitJoinedPermissions(const std::string& joinedPerms);
        };
    }
}


#endif
