#include "aex/fb/FBPermissions.hpp"
#include <functional>
#include <algorithm>
#include <sstream>

namespace aex
{
    namespace fb
    {
        std::string permissionToString(PERMISSION perm)
        {
            switch(perm)
            {
                case PERMISSION::PUBLIC_PROFILE: return "public_profile";
                case PERMISSION::USER_FRIENDS: return "user_friends";
                case PERMISSION::EMAIL: return "email";
                case PERMISSION::USER_ABOUT_ME: return "user_about_me";
                case PERMISSION::USER_ACTIONS_BOOKS: return "user_actions.books";
                case PERMISSION::USER_ACTIONS_FITNESS: return "user_actions.fitness";
                case PERMISSION::USER_ACTIONS_MUSIC: return "user_actions.music";
                case PERMISSION::USER_ACTIONS_NEWS: return "user_actions.news";
                case PERMISSION::USER_ACTIONS_VIDEO: return "user_actions.video";
                case PERMISSION::USER_BIRTHDAY: return "user_birthday";
                case PERMISSION::USER_EDUCATION_HISTORY: return "user_education_history";
                case PERMISSION::USER_EVENTS: return "user_events";
                case PERMISSION::USER_GAMES_ACTIVITY: return "user_games_activity";
                case PERMISSION::USER_HOMETOWN: return "user_hometown";
                case PERMISSION::USER_LIKES: return "user_likes";
                case PERMISSION::USER_LOCATION: return "user_location";
                case PERMISSION::USER_MANAGED_GROUPS: return "user_managed_groups";
                case PERMISSION::USER_PHOTOS: return "user_photos";
                case PERMISSION::USER_POSTS: return "user_posts";
                case PERMISSION::USER_RELATIONSHIPS: return "user_relationships";
                case PERMISSION::USER_RELATIONSHIP_DETAILS: return "user_relationship_details";
                case PERMISSION::USER_RELIGION_POLITICS: return "user_religion_politics";
                case PERMISSION::USER_TAGGED_PLACES: return "user_tagged_places";
                case PERMISSION::USER_VIDEOS: return "user_videos";
                case PERMISSION::USER_WEBSITE: return "user_website";
                case PERMISSION::USER_WORK_HISTORY: return "user_work_history";
                case PERMISSION::READ_CUSTOM_FRIENDLISTS: return "read_custom_friendlists";
                case PERMISSION::READ_INSIGHTS: return "read_insights";
                case PERMISSION::READ_AUDIENCE_NETWORK_INSIGHTS: return "read_audience_network_insights";
                case PERMISSION::READ_PAGE_MAILBOXES: return "read_page_mailboxes";
                case PERMISSION::MANAGE_PAGES: return "manage_pages";
                case PERMISSION::PUBLISH_PAGES: return "publish_pages";
                case PERMISSION::PUBLISH_ACTIONS: return "publish_actions";
                case PERMISSION::RSVP_EVENT: return "rsvp_event";
                case PERMISSION::PAGES_SHOW_LIST: return "pages_show_list";
                case PERMISSION::PAGES_MANAGE_CTA: return "pages_manage_cta";
                case PERMISSION::PAGES_MANAGE_INSTANT_ARTICLES: return "pages_manage_instant_articles";
                case PERMISSION::ADS_READ: return "ads_read";
                case PERMISSION::ADS_MANAGEMENT: return "ads_management";
                case PERMISSION::BUSINESS_MANAGEMENT: return "business_management";
                case PERMISSION::PAGES_MESSAGING: return "pages_messaging";
                case PERMISSION::PAGES_MESSAGING_SUBSCRIPTIONS: return "pages_messaging_subscriptions";
                case PERMISSION::PAGES_MESSAGING_PAYMENTS: return "pages_messaging_payments";
                case PERMISSION::PAGES_MESSAGING_PHONE_NUMBER : return "pages_messaging_phone_number";
                default: return "";
            }
        }

        PERMISSION stringToPermission(const std::string& permStr)
        {
            if(permStr == "public_profile") return PERMISSION::PUBLIC_PROFILE;
            if(permStr == "user_friends") return PERMISSION::USER_FRIENDS;
            if(permStr == "email") return PERMISSION::EMAIL;
            if(permStr == "user_about_me") return PERMISSION::USER_ABOUT_ME;
            if(permStr == "user_actions.books") return PERMISSION::USER_ACTIONS_BOOKS;
            if(permStr == "user_actions.fitness") return PERMISSION::USER_ACTIONS_FITNESS;
            if(permStr == "user_actions.music") return PERMISSION::USER_ACTIONS_MUSIC;
            if(permStr == "user_actions.news") return PERMISSION::USER_ACTIONS_NEWS;
            if(permStr == "user_actions.video") return PERMISSION::USER_ACTIONS_VIDEO;
            if(permStr == "user_birthday") return PERMISSION::USER_BIRTHDAY;
            if(permStr == "user_education_history") return PERMISSION::USER_EDUCATION_HISTORY;
            if(permStr == "user_events") return PERMISSION::USER_EVENTS;
            if(permStr == "user_games_activity") return PERMISSION::USER_GAMES_ACTIVITY;
            if(permStr == "user_hometown") return PERMISSION::USER_HOMETOWN;
            if(permStr == "user_likes") return PERMISSION::USER_LIKES;
            if(permStr == "user_location") return PERMISSION::USER_LOCATION;
            if(permStr == "user_managed_groups") return PERMISSION::USER_MANAGED_GROUPS;
            if(permStr == "user_photos") return PERMISSION::USER_PHOTOS;
            if(permStr == "user_posts") return PERMISSION::USER_POSTS;
            if(permStr == "user_relationships") return PERMISSION::USER_RELATIONSHIPS;
            if(permStr == "user_relationship_details") return PERMISSION::USER_RELATIONSHIP_DETAILS;
            if(permStr == "user_religion_politics") return PERMISSION::USER_RELIGION_POLITICS;
            if(permStr == "user_tagged_places") return PERMISSION::USER_TAGGED_PLACES;
            if(permStr == "user_videos") return PERMISSION::USER_VIDEOS;
            if(permStr == "user_website") return PERMISSION::USER_WEBSITE;
            if(permStr == "user_work_history") return PERMISSION::USER_WORK_HISTORY;
            if(permStr == "read_custom_friendlists") return PERMISSION::READ_CUSTOM_FRIENDLISTS;
            if(permStr == "read_insights") return PERMISSION::READ_INSIGHTS;
            if(permStr == "read_audience_network_insights") return PERMISSION::READ_AUDIENCE_NETWORK_INSIGHTS;
            if(permStr == "read_page_mailboxes") return PERMISSION::READ_PAGE_MAILBOXES;
            if(permStr == "manage_pages") return PERMISSION::MANAGE_PAGES;
            if(permStr == "publish_pages") return PERMISSION::PUBLISH_PAGES;
            if(permStr == "publish_actions") return PERMISSION::PUBLISH_ACTIONS;
            if(permStr == "rsvp_event") return PERMISSION::RSVP_EVENT;
            if(permStr == "pages_show_list") return PERMISSION::PAGES_SHOW_LIST;
            if(permStr == "pages_manage_cta") return PERMISSION::PAGES_MANAGE_CTA;
            if(permStr == "pages_manage_instant_articles") return PERMISSION::PAGES_MANAGE_INSTANT_ARTICLES;
            if(permStr == "ads_read") return PERMISSION::ADS_READ;
            if(permStr == "ads_management") return PERMISSION::ADS_MANAGEMENT;
            if(permStr == "business_management") return PERMISSION::BUSINESS_MANAGEMENT;
            if(permStr == "pages_messaging") return PERMISSION::PAGES_MESSAGING;
            if(permStr == "pages_messaging_subscriptions") return PERMISSION::PAGES_MESSAGING_SUBSCRIPTIONS;
            if(permStr == "pages_messaging_payments") return PERMISSION::PAGES_MESSAGING_PAYMENTS;
            if(permStr == "pages_messaging_phone_number") return PERMISSION::PAGES_MESSAGING_PHONE_NUMBER;

            return PERMISSION::NONE;
        }

        FBPermissionsGroup::FBPermissionsGroup()
        {

        }

        FBPermissionsGroup::~FBPermissionsGroup()
        {

        }

        void FBPermissionsGroup::addPermission(PERMISSION perm)
        {
            //std::string strPerm = permissionToString(perm);
            if(std::find(m_permissions.begin(), m_permissions.end(), perm) == m_permissions.end()) { // if not added
                    m_permissions.push_back(perm);
            }
        }

        std::string FBPermissionsGroup::getPermissionsJoined() const
        {
            std::stringstream ss;
            for(int i = 0 ; i < m_permissions.size() ; i++)
            {
                ss << permissionToString(m_permissions[i]);
                if( (i+1) < m_permissions.size())
                    ss << " ";
            }
            return ss.str();
        }

        std::vector<std::string> FBPermissionsGroup::getPermissionsAsStrings() const
        {
            std::vector<std::string> stringPerms;
            for(int i = 0 ; i < m_permissions.size() ; i++)
            {
                stringPerms.push_back(permissionToString(m_permissions[i]));
            }

            return stringPerms;
        }

        std::vector<std::string> FBPermissionsGroup::splitJoinedPermissions(const std::string& joinedPerms)
        {
            std::stringstream ss;
            ss << joinedPerms;
            std::vector<std::string> splitedPerms;
            std::string perm;
            while ( getline( ss, perm,' ') ) {
                splitedPerms.push_back(perm);
            }

            return splitedPerms;
        }

    }
}
