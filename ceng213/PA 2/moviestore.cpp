#include "moviestore.h"


MovieStore::MovieStore( ) //implemented, do not change
{
}


// IMPLEMENT
void
MovieStore::insert(const Movie & movie)
{
  primaryIndex.insert(movie.getID(), movie);
  Movie *film = &(primaryIndex.find(movie.getID())->data);
  SKey k(*film);
  secondaryIndex.insert(k, film);
  ternaryIndex.insert(k, film);
    
}


// IMPLEMENT
void
MovieStore::remove(const std::string & id)
{
    if (primaryIndex.find(id) == NULL)
    {
        return;
    }
    
    else
    {
        Movie film = primaryIndex.find(id)->data;
        SKey key(film);
        
        primaryIndex.remove(id);
        secondaryIndex.remove(key);
        ternaryIndex.remove(key);
    }
}


// IMPLEMENT
void
MovieStore::remove(const std::string & title,
                  const std::string & director)
{
    SKey key(title, director);
    
    if (ternaryIndex.find(key) == NULL)
    {
        return;
    }
    
    else
    {

        primaryIndex.remove(ternaryIndex.find(key)->data->getID());
        
        secondaryIndex.remove(key);
        ternaryIndex.remove(key);
    }
}


// IMPLEMENT
void
MovieStore::removeAllMoviesWithTitle(const std::string & title)
{
    SKey lower(title, "a");
    SKey upper(title, "{");
    
    std::list<MSTT::Node *> blockbuster = ternaryIndex.find(lower, upper);
    
    for (auto &picture: blockbuster)
    {
        remove(picture->data->getID());
    }
}


// IMPLEMENT
void
MovieStore::makeAvailable(const std::string & id)
{
    if (primaryIndex.find(id) != NULL)
    {
        (&primaryIndex.find(id)->data)->setAvailable();
    }
}


// IMPLEMENT
void
MovieStore::makeUnavailable(const std::string & title,
                           const std::string & director)
{
    SKey key(title, director);
    
    if (ternaryIndex.find(key) != NULL)
    {
        ternaryIndex.find(key)->data->setUnavailable();
    }

}


// IMPLEMENT
void
MovieStore::updateCompany(const std::string & director, 
                           const std::string & Company)
{
    SKey upper("z", director);
    SKey lower("a", director);
    
    
    std::list<MSTS::Node *> blockbuster = secondaryIndex.find(lower, upper);
    MSTP::Node *which;
    
    for (auto &picture: blockbuster)
    {
        which = primaryIndex.find(picture->data->getID());
        which->data.setCompany(Company);
    }
}


// IMPLEMENT
void
MovieStore::printMoviesWithID(const std::string & id1,
                              const std::string & id2,
                              unsigned short since) const
{
    std::list<MSTP::Node *> blockbuster = primaryIndex.find(id1, id2);
    
    for (auto &picture: blockbuster)
    {
        if (since <= ((picture->data).getYear()))
        {
            std::cout<<picture->data<<std::endl;
        }
        
    }
}


// IMPLEMENT
void
MovieStore::printMoviesOfDirector(const std::string & director,
                                  const std::string & first,
                                  const std::string & last) const
{
    std::list<MSTS::Node *> blockbuster;
    
    SKey upper(last, director);
    SKey lower(first, director);
    
    blockbuster = secondaryIndex.find(lower, upper);
    
    for (auto &picture: blockbuster)
    {
        std::cout<<*(picture->data)<<std::endl;
    }
    
    
}



void  // implemented, do not change
MovieStore::printPrimarySorted( ) const
{
  printPrimarySorted(primaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printPrimarySorted(MSTP::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printPrimarySorted(t->left);
  std::cout << t->data << std::endl;
  printPrimarySorted(t->right);
}


void  // implemented, do not change
MovieStore::printSecondarySorted( ) const
{
  printSecondarySorted(secondaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printSecondarySorted(MSTS::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printSecondarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printSecondarySorted(t->right);
}


void  // implemented, do not change
MovieStore::printTernarySorted( ) const
{
  printTernarySorted(ternaryIndex.getRoot());
}


void  // implemented, do not change
MovieStore::printTernarySorted(MSTT::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printTernarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printTernarySorted(t->right);
}

