// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <netdb.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>

#include "netdb_impl.h"

// Conversion between protocol names and numbers.

static uint8_t proto_get_id(const char *name) {
  if (strcasecmp(name, "tcp") == 0)
    return PORTSTR_TCP;
  if (strcasecmp(name, "udp") == 0)
    return PORTSTR_UDP;
  if (strcasecmp(name, "sctp") == 0)
    return PORTSTR_SCTP;
  if (strcasecmp(name, "dccp") == 0)
    return PORTSTR_DCCP;
  return 0;
}

static const char *proto_get_name(uint8_t protoid) {
  switch (protoid) {
    case PORTSTR_TCP:
      return "tcp";
    case PORTSTR_UDP:
      return "udp";
    case PORTSTR_SCTP:
      return "sctp";
    case PORTSTR_DCCP:
      return "dccp";
  }
  assert(0 && "Invalid protocol ID");
  return NULL;
}

// Allocates a new struct servent object based on an entry from the IANA
// port number registry.
static struct servent *create_servent(const char *entry, uint8_t protoid) {
  // Compute the number of aliases this entry has.
  size_t aliases_count = 1;
  for (const char *alias = portstr_get_next(entry);
       portstr_get_port(alias) == portstr_get_port(entry);
       alias = portstr_get_next(alias)) {
    if (portstr_match_protoid(alias, protoid))
      ++aliases_count;
  }

  // Allocate space to allocate the entry. We'll store the servent
  // object, followed by the pointers to the strings.
  struct servent *se = malloc(sizeof(*se) + aliases_count * sizeof(char *));
  if (se == NULL)
    return NULL;
  *se = (struct servent){
      .s_name = (char *)portstr_get_name(entry),
      .s_aliases = (char **)(se + 1),
      .s_port = portstr_get_port(entry),
      .s_proto = (char *)proto_get_name(protoid),
  };

  // Fill the array with aliases.
  const char **aliases = (const char **)(se + 1);
  for (const char *alias = portstr_get_next(entry);
       portstr_get_port(alias) == portstr_get_port(entry);
       alias = portstr_get_next(alias)) {
    if (portstr_match_protoid(alias, protoid))
      *aliases++ = portstr_get_name(alias);
  }
  *aliases = NULL;

  return se;
}

struct servent_entry {
  const char *entry;
  uint8_t protoid;
  struct servent *servent;
};

// Attempts to fetch a memoized copy of struct servent. If no copy is
// available, it creates a new copy through create_servent().
//
// TODO(ed): This could use a hash table. Maybe see if we could
// implement a generalized hash table for memoization.
static struct servent *get_servent(const char *entry, uint8_t protoid) {
  static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  static struct servent_entry *table = NULL;
  static size_t size = 0;

  // Search for an existing copy.
  pthread_mutex_lock(&lock);
  size_t last = 0;
  while (last < size && table[last].servent != NULL) {
    if (table[last].entry == entry && table[last].protoid == protoid) {
      pthread_mutex_unlock(&lock);
      return table[last].servent;
    }
    ++last;
  }

  // Grow the table if needed.
  if (last >= size) {
    size_t new_size = last * 2;
    if (new_size < 8)
      new_size = 8;
    struct servent_entry *new_table =
        realloc(table, new_size * sizeof(*new_table));
    if (new_table == NULL) {
      pthread_mutex_unlock(&lock);
      return NULL;
    }
    memset(new_table + size, '\0', (new_size - size) * sizeof(*new_table));
    table = new_table;
    size = new_size;
  }

  // Store the new entry.
  struct servent *new_servent = create_servent(entry, protoid);
  if (new_servent == NULL) {
    pthread_mutex_unlock(&lock);
    return NULL;
  }
  table[last].entry = entry;
  table[last].protoid = protoid;
  table[last++].servent = new_servent;
  pthread_mutex_unlock(&lock);
  return new_servent;
}

// Public API.

// Returns the first entry that matches both the name (or one of the
// aliases) and the protocol.
struct servent *getservbyname(const char *name, const char *proto) {
  uint8_t protoid = proto_get_id(proto);
  const char *first_entry = __iana_port_numbers;
  for (const char *entry = __iana_port_numbers; portstr_get_port(entry) != 0;
       entry = portstr_get_next(entry)) {
    if (portstr_match_protoid(entry, protoid)) {
      // Keep track of the first entry that has the same port number and
      // matches the protocol, as this data is used by create_servent().
      if (portstr_get_port(first_entry) != portstr_get_port(entry))
        first_entry = entry;

      // Match against the name.
      if (strcasecmp(portstr_get_name(entry), name) == 0)
        return get_servent(first_entry, protoid);
    }
  }
  return NULL;
}

// Returns the first entry that matches both the port number and the
// protocol.
struct servent *getservbyport(int port, const char *proto) {
  uint8_t protoid = proto_get_id(proto);
  for (const char *entry = __iana_port_numbers; portstr_get_port(entry) != 0;
       entry = portstr_get_next(entry)) {
    if (portstr_get_port(entry) == port &&
        portstr_match_protoid(entry, protoid))
      return get_servent(entry, protoid);
  }
  return NULL;
}
