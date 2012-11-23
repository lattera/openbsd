/*
 * dname.c -- Domain name handling.
 *
 * Copyright (c) 2001-2011, NLnet Labs. All rights reserved.
 *
 * See LICENSE for the license.
 *
 */


#include "config.h"

#include <sys/types.h>

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "dns.h"
#include "dname.h"
#include "query.h"

/**
 * The maximum number of labels is the maximum domain name, less 1 for
 * the root (len == 0) label, divided by two (minimum non-root label of
 * one character + length byte), then add back in one for the root label.
 */
#define	MAXLABELS	(((MAXDOMAINLEN - 1) / 2) + 1)

const dname_type *
dname_make(region_type *region, const uint8_t *name, int normalize)
{
	size_t name_size = 0;
	uint8_t label_offsets[MAXDOMAINLEN];
	uint8_t label_count = 0;
	const uint8_t *label = name;
	dname_type *result;
	ssize_t i;

	assert(name);

	while (1) {
		if (label_is_pointer(label))
			return NULL;

		label_offsets[label_count] = (uint8_t) (label - name);
		++label_count;
		name_size += label_length(label) + 1;

		if (label_is_root(label))
			break;

		label = label_next(label);
	}

	if (name_size > MAXDOMAINLEN)
		return NULL;

	assert(label_count <= MAXDOMAINLEN / 2 + 1);

	/* Reverse label offsets.  */
	for (i = 0; i < label_count / 2; ++i) {
		uint8_t tmp = label_offsets[i];
		label_offsets[i] = label_offsets[label_count - i - 1];
		label_offsets[label_count - i - 1] = tmp;
	}

	result = (dname_type *) region_alloc(
		region,
		(sizeof(dname_type)
		 + (label_count + name_size) * sizeof(uint8_t)));
	result->name_size = name_size;
	result->label_count = label_count;
	memcpy((uint8_t *) dname_label_offsets(result),
	       label_offsets,
	       label_count * sizeof(uint8_t));
	if (normalize) {
		uint8_t *dst = (uint8_t *) dname_name(result);
		const uint8_t *src = name;
		while (!label_is_root(src)) {
			ssize_t len = label_length(src);
			*dst++ = *src++;
			for (i = 0; i < len; ++i) {
				*dst++ = DNAME_NORMALIZE(*src++);
			}
		}
		*dst = *src;
	} else {
		memcpy((uint8_t *) dname_name(result),
		       name,
		       name_size * sizeof(uint8_t));
	}
	return result;
}


const dname_type *
dname_make_from_packet(region_type *region, buffer_type *packet,
		       int allow_pointers, int normalize)
{
	uint8_t buf[MAXDOMAINLEN + 1];
	if(!dname_make_wire_from_packet(buf, packet, allow_pointers))
		return 0;
	return dname_make(region, buf, normalize);
}

int
dname_make_wire_from_packet(uint8_t *buf, buffer_type *packet,
                       int allow_pointers)
{
	int done = 0;
	uint8_t visited[(MAX_PACKET_SIZE+7)/8];
	size_t dname_length = 0;
	const uint8_t *label;
	ssize_t mark = -1;

	memset(visited, 0, (buffer_limit(packet)+7)/8);

	while (!done) {
		if (!buffer_available(packet, 1)) {
/* 			error("dname out of bounds"); */
			return 0;
		}

		if (get_bit(visited, buffer_position(packet))) {
/* 			error("dname loops"); */
			return 0;
		}
		set_bit(visited, buffer_position(packet));

		label = buffer_current(packet);
		if (label_is_pointer(label)) {
			size_t pointer;
			if (!allow_pointers) {
				return 0;
			}
			if (!buffer_available(packet, 2)) {
/* 				error("dname pointer out of bounds"); */
				return 0;
			}
			pointer = label_pointer_location(label);
			if (pointer >= buffer_limit(packet)) {
/* 				error("dname pointer points outside packet"); */
				return 0;
			}
			buffer_skip(packet, 2);
			if (mark == -1) {
				mark = buffer_position(packet);
			}
			buffer_set_position(packet, pointer);
		} else if (label_is_normal(label)) {
			size_t length = label_length(label) + 1;
			done = label_is_root(label);
			if (!buffer_available(packet, length)) {
/* 				error("dname label out of bounds"); */
				return 0;
			}
			if (dname_length + length >= MAXDOMAINLEN+1) {
/* 				error("dname too large"); */
				return 0;
			}
			buffer_read(packet, buf + dname_length, length);
			dname_length += length;
		} else {
/* 			error("bad label type"); */
			return 0;
		}
	}

	if (mark != -1) {
		buffer_set_position(packet, mark);
	}

	return dname_length;
}

const dname_type *
dname_parse(region_type *region, const char *name)
{
	uint8_t dname[MAXDOMAINLEN];
	if(!dname_parse_wire(dname, name))
		return 0;
	return dname_make(region, dname, 1);
}

int dname_parse_wire(uint8_t* dname, const char* name)
{
	const uint8_t *s = (const uint8_t *) name;
	uint8_t *h;
	uint8_t *p;
	uint8_t *d = dname;
	size_t label_length;

	if (strcmp(name, ".") == 0) {
		/* Root domain.  */
		dname[0] = 0;
		return 1;
	}

	for (h = d, p = h + 1; *s; ++s, ++p) {
		if (p - dname >= MAXDOMAINLEN) {
			return 0;
		}

		switch (*s) {
		case '.':
			if (p == h + 1) {
				/* Empty label.  */
				return 0;
			} else {
				label_length = p - h - 1;
				if (label_length > MAXLABELLEN) {
					return 0;
				}
				*h = label_length;
				h = p;
			}
			break;
		case '\\':
			/* Handle escaped characters (RFC1035 5.1) */
			if (isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3])) {
				int val = (hexdigit_to_int(s[1]) * 100 +
					   hexdigit_to_int(s[2]) * 10 +
					   hexdigit_to_int(s[3]));
				if (0 <= val && val <= 255) {
					s += 3;
					*p = val;
				} else {
					*p = *++s;
				}
			} else if (s[1] != '\0') {
				*p = *++s;
			}
			break;
		default:
			*p = *s;
			break;
		}
	}

	if (p != h + 1) {
		/* Terminate last label.  */
		label_length = p - h - 1;
		if (label_length > MAXLABELLEN) {
			return 0;
		}
		*h = label_length;
		h = p;
	}

	/* Add root label.  */
	*h = 0;

	return p-dname;
}


const dname_type *
dname_copy(region_type *region, const dname_type *dname)
{
	return (dname_type *) region_alloc_init(
		region, dname, dname_total_size(dname));
}


const dname_type *
dname_partial_copy(region_type *region, const dname_type *dname, uint8_t label_count)
{
	if (!dname)
		return NULL;

	if (label_count == 0) {
		/* Always copy the root label.  */
		label_count = 1;
	}

	assert(label_count <= dname->label_count);

	return dname_make(region, dname_label(dname, label_count - 1), 0);
}


const dname_type *
dname_origin(region_type *region, const dname_type *dname)
{
	return dname_partial_copy(region, dname, dname->label_count - 1);
}


int
dname_is_subdomain(const dname_type *left, const dname_type *right)
{
	uint8_t i;

	if (left->label_count < right->label_count)
		return 0;

	for (i = 1; i < right->label_count; ++i) {
		if (label_compare(dname_label(left, i),
				  dname_label(right, i)) != 0)
			return 0;
	}

	return 1;
}


int
dname_compare(const void *vleft, const void *vright)
{
	const dname_type* left = (const dname_type*) vleft;
	const dname_type* right = (const dname_type*) vright;
	int result;
	uint8_t label_count;
	uint8_t i;

	assert(left);
	assert(right);

	if (left == right) {
		return 0;
	}

	label_count = (left->label_count <= right->label_count
		       ? left->label_count
		       : right->label_count);

	/* Skip the root label by starting at label 1.  */
	for (i = 1; i < label_count; ++i) {
		result = label_compare(dname_label(left, i),
				       dname_label(right, i));
		if (result) {
			return result;
		}
	}

	/* Dname with the fewest labels is "first".  */
	return (int) left->label_count - (int) right->label_count;
}


int
label_compare(const uint8_t *left, const uint8_t *right)
{
	int left_length;
	int right_length;
	size_t size;
	int result;

	assert(left);
	assert(right);

	assert(label_is_normal(left));
	assert(label_is_normal(right));

	left_length = label_length(left);
	right_length = label_length(right);
	size = left_length < right_length ? left_length : right_length;

	result = memcmp(label_data(left), label_data(right), size);
	if (result) {
		return result;
	} else {
		return (int) left_length - (int) right_length;
	}
}


uint8_t
dname_label_match_count(const dname_type *left, const dname_type *right)
{
	uint8_t i;

	assert(left);
	assert(right);

	for (i = 1; i < left->label_count && i < right->label_count; ++i) {
		if (label_compare(dname_label(left, i),
				  dname_label(right, i)) != 0)
		{
			return i;
		}
	}

	return i;
}

const char *
dname_to_string(const dname_type *dname, const dname_type *origin)
{
	static char buf[MAXDOMAINLEN * 5];
	return dname_to_string_r(dname, origin, buf);
}

const char *
dname_to_string_r(const dname_type *dname, const dname_type *origin,
	char* buf)
{
	size_t i;
	size_t labels_to_convert = 0;
	int absolute = 1;
	char *dst;
	const uint8_t *src;
	if (!dname) {
		*buf = '\0';
		return buf;
	}
	if (dname->label_count == 1) {
		strlcpy(buf, ".", sizeof(buf));
		return buf;
	}
	labels_to_convert = dname->label_count - 1;

	if (origin && dname_is_subdomain(dname, origin)) {
		int common_labels = dname_label_match_count(dname, origin);
		labels_to_convert = dname->label_count - common_labels;
		absolute = 0;
	}

	dst = buf;
	src = dname_name(dname);
	for (i = 0; i < labels_to_convert; ++i) {
		size_t len = label_length(src);
		size_t j;
		++src;
		for (j = 0; j < len; ++j) {
			uint8_t ch = *src++;
			if (isalnum(ch) || ch == '-' || ch == '_') {
				*dst++ = ch;
			} else if (ch == '.' || ch == '\\') {
				*dst++ = '\\';
				*dst++ = ch;
			} else {
				snprintf(dst, 5, "\\%03u", (unsigned int)ch);
				dst += 4;
			}
		}
		*dst++ = '.';
	}
	if (absolute) {
		*dst = '\0';
	} else {
		*--dst = '\0';
	}
	return buf;
}


const dname_type *
dname_make_from_label(region_type *region,
		      const uint8_t *label, const size_t length)
{
	uint8_t temp[MAXLABELLEN + 2];

	assert(length > 0 && length <= MAXLABELLEN);

	temp[0] = length;
	memcpy(temp + 1, label, length * sizeof(uint8_t));
	temp[length + 1] = '\000';

	return dname_make(region, temp, 1);
}


const dname_type *
dname_concatenate(region_type *region,
		  const dname_type *left,
		  const dname_type *right)
{
	uint8_t temp[MAXDOMAINLEN];

	assert(left->name_size + right->name_size - 1 <= MAXDOMAINLEN);

	memcpy(temp, dname_name(left), left->name_size - 1);
	memcpy(temp + left->name_size - 1, dname_name(right), right->name_size);

	return dname_make(region, temp, 0);
}


const dname_type *
dname_replace(region_type* region,
		const dname_type* name,
		const dname_type* src,
		const dname_type* dest)
{
	/* nomenclature: name is said to be <x>.<src>. x can be null. */
	dname_type* res;
	int x_labels = name->label_count - src->label_count;
	int x_len = name->name_size - src->name_size;
	int i;
	assert(dname_is_subdomain(name, src));

	/* check if final size is acceptable */
	if(x_len+dest->name_size > MAXDOMAINLEN)
		return NULL;

	res = (dname_type*)region_alloc(region, sizeof(dname_type) +
		(x_labels+dest->label_count + x_len+dest->name_size)
		*sizeof(uint8_t));
	res->name_size = x_len+dest->name_size;
	res->label_count = x_labels+dest->label_count;
	for(i=0; i<dest->label_count; i++)
		((uint8_t*)dname_label_offsets(res))[i] =
			dname_label_offsets(dest)[i] + x_len;
	for(i=dest->label_count; i<res->label_count; i++)
		((uint8_t*)dname_label_offsets(res))[i] =
			dname_label_offsets(name)[i - dest->label_count +
				src->label_count];
	memcpy((uint8_t*)dname_name(res), dname_name(name), x_len);
	memcpy((uint8_t*)dname_name(res)+x_len, dname_name(dest), dest->name_size);
	assert(dname_is_subdomain(res, dest));
	return res;
}

#ifndef FULL_PREHASH
/**
 * Make wildcard synthesis.
 *
 */
int
dname_make_wildcard(struct region *region,
		    struct dname const *dname,
		    struct dname const **wildcard)
{
	uint8_t name_size;
	uint8_t label_count;
	uint8_t *names;
	uint8_t *labels;
	struct dname *new_dname;
	unsigned int i;
	/*
	 * Checks:
	 *	dname label_count + 1 < MAXLABELS
	 * 	dname size + 2 < MAXDOMAINLEN
	 */
	if (dname->label_count > (MAXLABELS - 1)) {
		return EINVAL;
	}
	if (dname->name_size > (MAXDOMAINLEN - 2)) {
		return EINVAL;
	}

	label_count = dname->label_count + 1;
	name_size = dname->name_size + 2;
	new_dname = (struct dname *) region_alloc(region,
						  sizeof(dname_type) +
						  (label_count * sizeof(uint8_t)) +
						  (name_size * sizeof(uint8_t)));
	if (new_dname == NULL) {
		return ENOMEM;
	}
	new_dname->label_count = label_count;
	new_dname->name_size = name_size;
	labels = (uint8_t *) dname_label_offsets(new_dname);
	memcpy(labels, dname_label_offsets(dname),
	       dname->label_count * sizeof(uint8_t));
	for (i = 0; i < dname->label_count; i++) {
		labels[i] += 2;
	}
	labels[i] = 0;

	names = (uint8_t *) dname_name(new_dname);
	*names++ = '\001';
	*names++ = '*';
	memcpy(names, dname_name(dname),
	       dname->name_size * sizeof(uint8_t));
	*wildcard = new_dname;
	return 0;
}
#endif
