#include "SocialNetwork.h"

#include <iostream>

SocialNetwork::SocialNetwork() {

}

void SocialNetwork::addProfile(const std::string &firstname, const std::string &lastname, const std::string &email) 
{
    Profile *p = new Profile(firstname, lastname, email);
    
    profiles.insertSorted(*p);
    
    delete p;
    
    
}

void SocialNetwork::addPost(const std::string &message, const std::string &email) 
{
    Post p = Post(message);
    posts.insertAtTheTail(p);
    
    
    Node<Profile> *curr = profiles.getFirstNode();
    
    
    while (curr != nullptr)
    {
        if (curr->data.getEmail() == email)
        {
            
            curr->data.addPost(&(posts.getLastNode()->data));
            
            
            
            break;
        }
        
        curr = curr->next;
    }
    
   
   
}

void SocialNetwork::deleteProfile(const std::string &email) 
{
    Node<Profile> *p= (profiles.getFirstNode());
    
        while (p != nullptr)
        {
            if (p->data.getEmail() == email)
            {
                break;
            }
            
            p = p->next;
        }
    
    
    Node<Profile *> *mate = (p->data.getFriends()).getFirstNode();
    Node<Profile *> *mdel;
    while (mate != nullptr)
    {
        mdel = (mate->data->getFriends()).getFirstNode();
        
        while (mdel != nullptr)
        {
            if (*(mdel->data) == p->data)
            {
                mate->data->dropFriend(&(p->data));
                break;
            }
            
            mdel = mdel->next;
        }
        
        mate = mate->next;
    }
    
    Node<Profile> *other;
    Node<Post *> *posta = p->data.getPosts().getFirstNode();
    Node <Post *> *liked;
    
    while (posta != nullptr)
    {
        other = (profiles.getFirstNode());
        
        while (other != nullptr)
        {
            liked = other->data.getLikes().getFirstNode();
            
            while (liked != nullptr)
            {
                if (posta->data == liked->data)
                {
                    other->data.dropLike(posta->data);
                    break;
                }
                
                liked = liked->next;
            }
            
            other = other->next;
        }
        
        posta = posta->next;
    }
    
    Node<Post *> *nope = p->data.getPosts().getFirstNode();
    Node<Post> *gez;
    
    while (nope != nullptr)
    {
        gez = posts.getFirstNode();
        
        while (gez != nullptr)
        {
            if (*(nope->data) == gez->data)
            {
                posts.removeNode(*(nope->data));
                break;
            }
            
            gez = gez->next;
        }
        
        nope = nope->next;
    }
    
      
    p->data.getPosts().removeAllNodes();
    p->data.getLikes().removeAllNodes();
    p->data.getFriends().removeAllNodes();
    
    profiles.removeNode(p->data);
    
    

}

void SocialNetwork::makeFriends(const std::string &email1, const std::string &email2) 
{
    Node<Profile> *curr = profiles.getFirstNode();
    
    while (curr != nullptr)
    {
        if (curr->data.getEmail() == email1)
        {
            break;
        }
        
        curr = curr->next;
    }
    
    Node<Profile> *tmp = profiles.getFirstNode();
    
    while (tmp != nullptr)
    {
        if (tmp->data.getEmail() == email2)
        {
            break;
        }
        
        tmp = tmp->next;
    }
    
    curr->data.addFriend(&(tmp->data));
    
    tmp->data.addFriend(&(curr->data));
}

void SocialNetwork::likePost(int postId, const std::string &email) 
{
    Node<Profile> *who = profiles.getFirstNode();
    
    while (who != nullptr)
    {
        if (who->data.getEmail() == email)
        {
            break;
        }
        
        who = who->next;
    }
    
    Node<Post> *which = posts.getFirstNode();
    
    while (which != nullptr)
    {
        if (which->data.getPostId() == postId)
        {
            break;
        }
        
        which = which->next;
    }
    
    who->data.addLike(&(which->data));
}

void SocialNetwork::unlikePost(int postId, const std::string &email) 
{
    Node<Profile> *who = profiles.getFirstNode();
    
    while (who != nullptr)
    {
        if (who->data.getEmail() == email)
        {
            break;
        }
        
        who = who->next;
    }
    
    Node<Post> *which = posts.getFirstNode();
    
    while (which != nullptr)
    {
        if (which->data.getPostId() == postId)
        {
            break;
        }
        
        which = which->next;
    }
    
    who->data.dropLike(&(which->data));
}

LinkedList<Profile *> SocialNetwork::getMutualFriends(const std::string &email1, const std::string &email2) 
{
    Node<Profile> *first = profiles.getFirstNode();
    
    while (first != nullptr)
    {
        if (first->data.getEmail() == email1)
        {
            break;
        }
        
        first = first->next;
    }
    
    Node<Profile> *second = profiles.getFirstNode();
    
    while (second != nullptr)
    {
        if (second->data.getEmail() == email2)
        {
            break;
        }
        
        second = second->next;
    }    
    
   Node<Profile *> *mate1 = (first->data.getFriends()).getFirstNode();
   Node<Profile *> *mate2 = (second->data.getFriends()).getFirstNode();
   
   LinkedList<Profile *> thelist;
   
   while (mate1 != nullptr)
   {
       mate2 = (second->data.getFriends()).getFirstNode();
       while (mate2 != nullptr)
       {
       
       
            if (mate1->data == mate2->data)
            {
                thelist.insertAtTheTail(mate2->data);
            }
       
            mate2 = mate2->next;
       }
       
       mate1 = mate1->next;
   }
   
   
   return thelist;
   
}

LinkedList<Post *> SocialNetwork::getListOfMostRecentPosts(const std::string &email, int k) 
{
    Node<Profile> *pro = profiles.getFirstNode();
    
    while (pro != nullptr)
    {
        if (pro->data.getEmail() == email)
        {
            break;
        }
        
        pro = pro->next;
    }
    
    LinkedList<Post *> thelist;
    
    Node<Post *> *posta = (pro->data.getPosts()).getLastNode();
    
    
    for (int i=0; i<k; i++)
    {
        if (posta != nullptr)
        {
            thelist.insertAtTheTail(posta->data);
            
            posta = posta->prev;
        }
        
        else
        {
            break;
        }
    }
    
    return thelist;
}

void SocialNetwork::print() const {
    std::cout << "# Printing the social network ..." << std::endl;

    std::cout << "# Number of profiles is " << profiles.getNumberOfNodes() << ":" << std::endl;
    profiles.print();

    std::cout << "# Number of posts is " << posts.getNumberOfNodes() << ":" << std::endl;
    posts.print();

    std::cout << "# Printing is done." << std::endl;
}
