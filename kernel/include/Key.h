/**
 *  \file Key.h    \brief Keys to cache lookup of attribute strings.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 *
 */

#ifndef IMP_KEY_H
#define IMP_KEY_H

#include "macros.h"
#include "exception.h"
#include "utility.h"
#include "internal/key_helpers.h"

#include <map>
#include <vector>

IMP_BEGIN_NAMESPACE

/**
   Define a new key type.

   It defines two public types Name, which is an instantiation of KeyBase and
   Names which is a vector of Name.

   \param[in] Name The name for the new type.
   \param[in] Tag A (hopefully) unique integer to define this key type.

   \note We define a new class rather than use a typedef since SWIG has a
   bug dealing with names that start with ::. A fix has been commited to SVN
   for swig.

   \note The name in the typedef would have to start with ::IMP so it
   could be used out of the IMP namespace.
 */
#ifndef IMP_DOXYGEN
#define IMP_DECLARE_KEY_TYPE(Name, Tag)                                 \
  struct Name: public ::IMP::KeyBase<Tag> {                             \
  typedef ::IMP::KeyBase<Tag> P;                                        \
  typedef Name This;                                                    \
  Name(){};                                                             \
  Name(unsigned int i): P(i){}                                          \
  Name(const char *nm): P(nm){}                                         \
  static Name add_alias(Name nm, const char *new_name) {                \
    ::IMP::KeyBase<Tag>:: add_alias(nm, new_name);                      \
    IMP_assert(Name(new_name) == nm, "Keys don't match after alias.");   \
    return Name(new_name);                                              \
  }                                                                     \
};                                                                      \
typedef std::vector<Name> Name##s
#else
#define IMP_DECLARE_KEY_TYPE(Name, Tag)                                 \
  /** A string based identifier.*/                                      \
  struct Name: public ::IMP::KeyBase<ID> {                              \
  typedef ::IMP::KeyBase<ID> P;                                         \
  typedef Name This;                                                    \
  Name(){};                                                             \
  IMP_NO_DOXYGEN(Name(unsigned int i): P(i){})                          \
  Name(const char *nm): P(nm){}                                         \
  /** Define the string new_name to refer to the same key as nm. */     \
  static Name add_alias(Name nm, const char *new_name);                 \
};                                                                      \
typedef std::vector<Name> Name##s
#endif

//! A base class for  Keys
/** This class does internal caching of the strings to accelerate the
    name lookup. It is better to create an Key and reuse it
    rather than recreate it many times from strings.

    If you use this with a new type, you must add a new definition of
    attribute_table_index. Yes, this is an evil hack, but I couldn't
    get linking to work with static members of the template class.

    The keys in IMP maintain a cached mapping between strings and indexes.
    This mapping is global--that is all IMP Models and Particles in the
    same program use the same mapping for each type of key. The type of
    the key is determined by an integer which should be unique for
    each type. If the integer is not unique, everything works, just
    more memory is wasted and types are interconvertible

    Keys used for storing attributes in particles should never be statically
    initialized. While this is annoying, statically initializing them is bad,
    as unused attribute keys can result in wasted memory in each particle.

    \note Keys objects are ordered.
 */
template <unsigned int ID>
class KeyBase: public NullDefault, public Comparable
{
  int str_;

  static const internal::KeyData::Map& get_map()
  {
    return IMP::internal::get_key_data(ID).get_map();
  }
  static const internal::KeyData::RMap& get_rmap() {
    return IMP::internal::get_key_data(ID).get_rmap();
  }


  static unsigned int find_index(std::string sc) {
    if (get_map().find(sc) == get_map().end()) {
      return IMP::internal::get_key_data(ID).add_key(sc);
    } else {
      return get_map().find(sc)->second;
    }
  }
private:
 bool is_default() const;
public:
#ifndef DOXYGEN
  static unsigned int get_ID() {
    return ID;
  }

  static const std::string get_string(int i)
  {
    if (static_cast<unsigned int>(i)
        < get_rmap().size()) {
      return get_rmap()[i];
    } else {
      IMP_WARN("Corrupted Key Table asking for key " << i
               << " with a table of size " << get_rmap().size());
      std::ostringstream oss;
      oss<< "Invalid index " << i;
      return oss.str();
    }
  }

  typedef KeyBase<ID> This;
#endif

  //! make a default key in a well-defined null state
  KeyBase(): str_(-1) {}


  //! Generate a key from the given string
  /** This operation can be expensive, so please cache the result.*/
  explicit KeyBase(const char *c) {
    str_= find_index(c);
  }

#ifndef DOXYGEN
  explicit KeyBase(unsigned int i): str_(i) {
    IMP_assert(str_ >= 0, "Invalid initializer " << i);
    // cannot check here as we need a past end iterator
  }
#endif

  //! Return true if there already is a key with that string
  static bool get_key_exists(std::string sc) {
    return get_map().find(sc) != get_map().end();
  }

  //! Turn a key into a pretty string
  const std::string get_string() const {
    if (is_default()) return std::string("NULL");
    return get_string(str_);
    //return str_;
  }

  IMP_COMPARISONS_1(str_)

  void show(std::ostream &out = std::cout) const {
    out << "\"" << get_string() << "\"";
  }

  //! Make new_name an alias for old_key
  /** Afterwards
      \code
      KeyBase<ID>(old_key.get_string()) == KeyBase<ID>(new_name)
      \endcode
   */
  static KeyBase<ID> add_alias(KeyBase<ID> old_key, std::string new_name) {
    IMP_assert(get_map().find(new_name) == get_map().end(),
               "The name is already taken with an existing key or alias");
    IMP::internal::get_key_data(ID).add_alias(new_name, old_key.get_index());
    return KeyBase<ID>(new_name.c_str());
  }

#ifndef DOXYGEN
  unsigned int get_index() const {
    IMP_assert(!is_default(),
               "Cannot get index on defaultly constructed Key");
    return str_;
  }
#endif

  //! Show all the keys of this type
  static void show_all(std::ostream &out);

  //! Get a list of all of the keys of this type
  /**
     This can be used to check for typos and similar keys.
   */
  static std::vector<std::string> get_all_strings();

  //! Get the total number of keys of this type
  /**
     This is mostly for debugging to makes sure that there are no extra
     keys created.
   */
  static unsigned int get_number_unique() {
    return get_map().size();
  }

#ifndef SWIG
  /** \todo These should be protected, I'll try to work how
   */
  This& operator++() {
    ++str_;
    return *this;
  }
  This& operator--() {
    --str_;
    return *this;
  }
  This operator+(int o) const {
    This c=*this;
    c.str_+= o;
    return c;
  }
#endif
};


template <unsigned int ID>
std::ostream &operator<<(std::ostream &out, KeyBase<ID> k) {
  k.show(out);
  return out;
}

template <unsigned int ID>
inline bool KeyBase<ID>::is_default() const
{
  return str_==-1;
}


template <unsigned int ID>
inline void KeyBase<ID>::show_all(std::ostream &out)
{
  internal::get_key_data(ID).show(out);
}

template <unsigned int ID>
std::vector<std::string> KeyBase<ID>::get_all_strings()
{
  std::vector<std::string> str;
  for (internal::KeyData::Map::const_iterator it= get_map().begin();
       it != get_map().end(); ++it) {
    str.push_back(it->first);
  }
  return str;
}

IMP_END_NAMESPACE

#endif  /* IMP_KEY_H */
