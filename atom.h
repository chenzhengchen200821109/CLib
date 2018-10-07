//
// Created by chenzheng on 21/01/2017.
//

#ifndef TEST_ATOM_H
#define TEST_ATOM_H

extern int Atom_length(const char *str);
extern const char *Atom_new(const char *str, int len);
extern const char *Atom_string(const char *str);
extern const char *Atom_int(long n);
extern void Atom_free(const char *str);
extern void Atom_reset(void);
extern void Atom_vload(const char *str, ...);
extern void Atom_aload(const char *strs[]);
extern const char *Atom_add(const char *str, int len);

#endif //TEST_ATOM_H
