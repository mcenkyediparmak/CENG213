#include "Profile.h"

Profile::Profile(const std::string &email, const std::string &username, SubscriptionPlan plan)
{
    this->email = email;
    this->username = username;
    this->plan = plan;
}

const std::string &Profile::getUsername() const
{
    return this->username;
}

const std::string &Profile::getEmail() const
{
    return this->email;
}

SubscriptionPlan Profile::getPlan() const
{
    return this->plan;
}

LinkedList<Playlist> &Profile::getPlaylists()
{
    return this->playlists;
}

LinkedList<Profile *> &Profile::getFollowings()
{
    return this->following;
}

LinkedList<Profile *> &Profile::getFollowers()
{
    return this->followers;
}

void Profile::setPlan(SubscriptionPlan plan)
{
    this->plan = plan;
}

void Profile::followProfile(Profile *profile)
{
    (this->following).insertAtTheEnd(profile);
    profile->getFollowers().insertAtTheEnd(this);
}

void Profile::unfollowProfile(Profile *profile)
{
    (this->following).removeNode(profile);
    profile->getFollowers().removeNode(this);
}

void Profile::createPlaylist(const std::string &playlistName)
{
    (this->playlists).insertAtTheEnd(Playlist(playlistName));
}

void Profile::deletePlaylist(int playlistId)
{
    Node<Playlist> *curr = playlists.getFirstNode();
    do
    {
        if (curr->data.getPlaylistId() == playlistId)
        {
            playlists.removeNode(curr);
            break;
        }
        curr = curr->next;
    } while (curr != playlists.getFirstNode());
}

void Profile::addSongToPlaylist(Song *song, int playlistId)
{
    Node<Playlist> *curr = playlists.getFirstNode();
    do
    {
        if (curr->data.getPlaylistId() == playlistId)
        {
            curr->data.addSong(song);
            break;
        }
        curr = curr->next;
    } while (curr != playlists.getFirstNode());
}

void Profile::deleteSongFromPlaylist(Song *song, int playlistId)
{
    Node<Playlist> *curr = playlists.getFirstNode();
    do
    {
        if (curr->data.getPlaylistId() == playlistId)
        {
            curr->data.dropSong(song);
            break;
        }
        curr = curr->next;
    } while (curr != playlists.getFirstNode());
}

Playlist *Profile::getPlaylist(int playlistId)
{
    Node<Playlist> *curr = playlists.getFirstNode();
    do
    {
        if (curr->data.getPlaylistId() == playlistId)
        {
            Playlist *PL = &curr->data;
            return PL;
        }
        curr = curr->next;
    } while (curr != playlists.getFirstNode());
}

LinkedList<Playlist *> Profile::getSharedPlaylists()
{
    Node<Profile *> *currUser = following.getFirstNode();
    LinkedList<Playlist *> sharedPlaylists;
    do
    {
        Node<Playlist> *currPL = (*currUser->data).getPlaylists().getFirstNode();
        do
        {
            if (currPL->data.isShared())
            {
                sharedPlaylists.insertAtTheEnd(&currPL->data);
            }
            currPL = currPL->next;

        } while (currPL != (*currUser->data).getPlaylists().getFirstNode());
        currUser = currUser->next;

    } while (currUser != following.getFirstNode());
}

void Profile::shufflePlaylist(int playlistId, int seed)
{
    Playlist *PL = this->getPlaylist(playlistId);
    PL->shuffle(seed);
}

void Profile::sharePlaylist(int playlistId)
{
    Playlist *PL = this->getPlaylist(playlistId);
    PL->setShared(true);
}

void Profile::unsharePlaylist(int playlistId)
{
    Playlist *PL = this->getPlaylist(playlistId);
    PL->setShared(false);
}

bool Profile::operator==(const Profile &rhs) const
{
    return this->email == rhs.email && this->username == rhs.username && this->plan == rhs.plan;
}

bool Profile::operator!=(const Profile &rhs) const
{
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Profile &profile)
{
    os << "email: " << profile.email << " |";
    os << " username: " << profile.username << " |";
    if (profile.plan == free_of_charge)
    {
        os << " plan: "
           << "free_of_charge"
           << " |";
    }
    else if (profile.plan == premium)
    {
        os << " plan: "
           << "premium"
           << " |";
    }
    else
    {
        os << " plan: "
           << "undefined"
           << " |";
    }

    os << " playlists: [";
    Node<Playlist> *firstPlaylistNode = profile.playlists.getFirstNode();
    Node<Playlist> *playlistNode = firstPlaylistNode;
    if (playlistNode)
    {
        do
        {
            os << playlistNode->data;
            if (playlistNode->next != firstPlaylistNode)
                os << ", ";
            playlistNode = playlistNode->next;
        } while (playlistNode != firstPlaylistNode);
    }
    os << "] |";
    os << " following: [";
    Node<Profile *> *firstProfileNode = profile.following.getFirstNode();
    Node<Profile *> *profileNode = firstProfileNode;
    if (profileNode)
    {
        do
        {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode)
                os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "] |";
    os << " followers: [";
    firstProfileNode = profile.followers.getFirstNode();
    profileNode = firstProfileNode;
    if (profileNode)
    {
        do
        {
            os << profileNode->data->getEmail();
            if (profileNode->next != firstProfileNode)
                os << ", ";
            profileNode = profileNode->next;
        } while (profileNode != firstProfileNode);
    }
    os << "]";

    return os;
}
