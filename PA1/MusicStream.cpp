#include "MusicStream.h"

#include <iostream>

void MusicStream::addProfile(const std::string &email, const std::string &username, SubscriptionPlan plan)
{
    this->profiles.insertAtTheEnd(Profile(email, username, plan));
}

void MusicStream::deleteProfile(const std::string &email)
{
    Profile user = getProfileByEmail(email);
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    do
    {
        Node<Profile *> *currFollower = currProfile->data.getFollowers().getFirstNode();
        do
        {
            if (currFollower->data == &user)
            {
                currProfile->data.getFollowers().removeNode(&user);
            }
            currFollower = currFollower->next;

        } while (currFollower != currProfile->data.getFollowers().getFirstNode());
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
    this->profiles.removeNode(user);
}

void MusicStream::addArtist(const std::string &artistName)
{
    this->artists.insertAtTheEnd(Artist(artistName));
}

void MusicStream::addAlbum(const std::string &albumName, int artistId)
{
    Node<Artist> *currArtist = this->artists.getFirstNode();
    do
    {
        if (currArtist->data.getArtistId() == artistId)
        {

            Album newAlbum = Album(albumName);
            currArtist->data.addAlbum(&newAlbum);
            this->albums.insertAtTheEnd(newAlbum);
            break;
        }
        currArtist = currArtist->next;
    } while (currArtist != this->artists.getFirstNode());
}

void MusicStream::addSong(const std::string &songName, int songDuration, int albumId)
{
    Node<Album> *currAlbum = this->albums.getFirstNode();
    do
    {
        if (currAlbum->data.getAlbumId() == albumId)
        {
            Song newSong = Song(songName, songDuration);
            this->songs.insertAtTheEnd(newSong);
            currAlbum->data.addSong(&newSong);
        }
        currAlbum = currAlbum->next;
    } while (currAlbum != this->albums.getFirstNode());
}

void MusicStream::followProfile(const std::string &email1, const std::string &email2)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    Profile follower, following;
    int breaker = 0;
    do
    {
        if (currProfile->data.getEmail() == email1)
        {
            follower = currProfile->data;
            breaker++;
        }
        if (currProfile->data.getEmail() == email2)
        {
            following = currProfile->data;
            breaker++;
        }
        if (breaker == 2)
        {
            break;
        }

    } while (currProfile != this->profiles.getFirstNode());
    follower.followProfile(&following);
}

void MusicStream::unfollowProfile(const std::string &email1, const std::string &email2)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    Profile follower, following;
    int breaker = 0;
    do
    {
        if (currProfile->data.getEmail() == email1)
        {
            follower = currProfile->data;
            breaker++;
        }
        if (currProfile->data.getEmail() == email2)
        {
            following = currProfile->data;
            breaker++;
        }
        if (breaker == 2)
        {
            break;
        }

    } while (currProfile != this->profiles.getFirstNode());
    follower.unfollowProfile(&following);
}

void MusicStream::createPlaylist(const std::string &email, const std::string &playlistName)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    do
    {
        if (currProfile->data.getEmail() == email)
        {
            currProfile->data.createPlaylist(playlistName);
            break;
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

void MusicStream::deletePlaylist(const std::string &email, int playlistId)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    do
    {
        if (currProfile->data.getEmail() == email)
        {
            currProfile->data.deletePlaylist(playlistId);
            break;
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

void MusicStream::addSongToPlaylist(const std::string &email, int songId, int playlistId)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    Node<Song> *currSong = this->songs.getFirstNode();
    Song newSong;
    do
    {
        if (currSong->data.getSongId() == songId)
        {
            newSong = currSong->data;
            break;
        }
        currSong = currSong->next;
    } while (currSong != this->songs.getFirstNode());
    do
    {
        if (currProfile->data.getEmail() == email)
        {
            currProfile->data.getPlaylist(playlistId)->addSong(&newSong);
            break;
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

void MusicStream::deleteSongFromPlaylist(const std::string &email, int songId, int playlistId)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    Node<Song> *currSong = this->songs.getFirstNode();
    Song newSong;
    do
    {
        if (currSong->data.getSongId() == songId)
        {
            newSong = currSong->data;
            break;
        }
        currSong = currSong->next;
    } while (currSong != this->songs.getFirstNode());
    do
    {
        if (currProfile->data.getEmail() == email)
        {
            currProfile->data.getPlaylist(playlistId)->dropSong(&newSong);
            break;
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

LinkedList<Song *> MusicStream::playPlaylist(const std::string &email, Playlist *playlist)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();
    LinkedList<Song *> Songs;
    do
    {
        if (currProfile->data.getEmail() == email)
        {
            if (currProfile->data.getPlan())
            {
                return playlist->getSongs();
            }
            else
            {
                (playlist->getSongs()).insertAsEveryKthNode(&Song::ADVERTISEMENT_SONG, 2);
                return (playlist->getSongs());
            }
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

Playlist *MusicStream::getPlaylist(const std::string &email, int playlistId)
{
    Node<Profile> *currProfile = this->profiles.getFirstNode();

    do
    {
        if (currProfile->data.getEmail() == email)
        {
            return currProfile->data.getPlaylist(playlistId);
        }
        currProfile = currProfile->next;
    } while (currProfile != this->profiles.getFirstNode());
}

LinkedList<Playlist *> MusicStream::getSharedPlaylists(const std::string &email)
{
    Profile user = getProfileByEmail(email);
    Node<Profile *> *followings = user.getFollowings().getFirstNode();
    LinkedList<Playlist *> sharedPlaylists;
    do
    {
        Node<Playlist> *currPL = followings->data->getPlaylists().getFirstNode();
        do
        {
            if (currPL->data.isShared())
            {
                sharedPlaylists.insertAtTheEnd(&currPL->data);
            }
            currPL = currPL->next;

        } while (currPL != followings->data->getPlaylists().getFirstNode());
        followings = followings;
    } while (followings != user.getFollowings().getFirstNode());
}

void MusicStream::shufflePlaylist(const std::string &email, int playlistId, int seed)
{
    Profile user = getProfileByEmail(email);
    Playlist *shufflingPL = user.getPlaylist(playlistId);
    shufflingPL->shuffle(seed);
}

void MusicStream::sharePlaylist(const std::string &email, int playlistId)
{
    Profile user = getProfileByEmail(email);
    user.getPlaylist(playlistId)->setShared(1);
}

void MusicStream::unsharePlaylist(const std::string &email, int playlistId)
{
    Profile user = getProfileByEmail(email);
    user.getPlaylist(playlistId)->setShared(0);
}

void MusicStream::subscribePremium(const std::string &email)
{
    Profile user = getProfileByEmail(email);
    user.setPlan(premium);
}

void MusicStream::unsubscribePremium(const std::string &email)
{
    Profile user = getProfileByEmail(email);
    user.setPlan(free_of_charge);
}

void MusicStream::print() const
{
    std::cout << "# Printing the music stream ..." << std::endl;

    std::cout << "# Number of profiles is " << this->profiles.getSize() << ":" << std::endl;
    this->profiles.print();

    std::cout << "# Number of artists is " << this->artists.getSize() << ":" << std::endl;
    this->artists.print();

    std::cout << "# Number of albums is " << this->albums.getSize() << ":" << std::endl;
    this->albums.print();

    std::cout << "# Number of songs is " << this->songs.getSize() << ":" << std::endl;
    this->songs.print();

    std::cout << "# Printing is done." << std::endl;
}
