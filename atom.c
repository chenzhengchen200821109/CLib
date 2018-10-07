//
// Created by chenzheng on 21/01/2017.
//

#include "atom.h"
#include <stdarg.h>
#include<string.h>
#include "assert_v.h"
#include <limits.h>
#include "mem.h"

#define NELEMS(x) ((sizeof(x))/(sizeof((x)[0])))

static struct atom {
    struct atom *link;
    int len;
    char *str;
} *buckets[2048];

/* generate 256 random numbers */
static unsigned long scatter[] = {
        1804289383, 846930886, 1681692777, 1714636915, 1957747793, 424238335,
        719885386, 1649760492, 596516649, 1189641421, 1025202362, 1350490027,
        783368690, 1102520059, 2044897763, 1967513926, 1365180540, 1540383426,
        304089172, 1303455736, 521595368, 294702567, 1726956429, 336465782,
        861021530, 278722862, 233665123, 2145174067, 468703135, 1101513929,
        1801979802, 1315634022, 635723058, 1369133069, 1125898167, 1059961393,
        2089018456, 628175011, 1656478042, 1131176229, 1653377373, 859484421,
        1914544919, 608413784, 756898537, 1734575198, 1973594324, 149798315,
        2038664370, 1129566413, 184803526, 412776091, 1424268980, 1911759956,
        749241873, 137806862, 982906996, 135497281, 511702305, 2084420925,
        1937477084, 1827336327, 572660336, 1159126505, 805750846, 1632621729,
        1100661313, 1433925857, 1141616124, 939819582, 2001100545, 1998898814,
        1548233367, 610515434, 1585990364, 1374344043, 760313750, 1477171087,
        356426808, 945117276, 1889947178, 1780695788, 709393584, 491705403,
        1918502651, 752392754, 1474612399, 2053999932, 1264095060, 1411549676,
        1843993368, 943947739, 1984210012, 855636226, 1749698586, 1469348094,
        1956297539, 1036140795, 463480570, 2040651434, 1975960378, 317097467,
        1892066601, 1376710097, 927612902, 1330573317, 603570492, 1687926652,
        660260756, 959997301, 485560280, 402724286, 593209441, 1194953865,
        894429689, 364228444, 1947346619, 221558440, 270744729, 1063958031,
        1633108117, 2114738097, 2007905771, 1469834481, 822890675, 1610120709,
        791698927, 631704567, 498777856, 1255179497, 524872353, 327254586,
        1572276965, 269455306, 1703964683, 352406219, 1600028624, 160051528,
        2040332871, 112805732, 1120048829, 378409503, 515530019, 1713258270,
        1573363368, 1409959708, 2077486715, 1373226340, 1631518149, 200747796,
        289700723, 1117142618, 168002245, 150122846, 439493451, 990892921,
        1760243555, 1231192379, 1622597488, 111537764, 338888228, 2147469841,
        438792350, 1911165193, 269441500, 2142757034, 116087764, 1869470124,
        155324914, 1982275856, 1275373743, 387346491, 350322227, 841148365,
        1960709859, 1760281936, 771151432, 1186452551, 1244316437, 971899228,
        1476153275, 213975407, 1139901474, 1626276121, 653468858, 2130794395,
        1239036029, 1884661237, 1605908235, 1350573793, 1605894428, 1789366143,
        1987231011, 1875335928, 1784639529, 2103318776, 1597322404, 1939964443,
        2112255763, 1432114613, 1067854538, 352118606, 1782436840, 1909002904,
        165344818, 1395235128, 532670688, 1351797369, 492067917, 1504569917,
        680466996, 706043324, 496987743, 159259470, 1359512183, 480298490,
        1398295499, 1096689772, 2086206725, 601385644, 1172755590, 1544617505,
        243268139, 1012502954, 1272469786, 2027907669, 968338082, 722308542,
        1820388464, 933110197, 740759355, 1285228804, 1789376348, 502278611,
        1450573622, 1037127828, 1034949299, 654887343, 1529195746, 392035568,
        1335354340, 889023311, 1494613810, 1447267605, 1369321801, 745425661,
        396473730, 1308044878, 1346811305, 1569229320, 705178736, 1590079444,
        434248626, 1977648522, 1470503465, 1402586708
};

const char *Atom_string(const char *str) {
    assert(str); /* assert if str is a null pointer */
    return Atom_new(str, strlen(str));
}

const char *Atom_int(long n) {
    char str[43];
    char *s = str + sizeof(str);
    unsigned long m;

    if (n == LONG_MIN)
        m = LONG_MAX + 1UL;
    else if (n < 0)
        m = -n;
    else
        m = n;
    do
        *--s = m%10 + '0';
    while ((m /= 10) > 0);

    if (n < 0)
        *--s = '-';
    return Atom_new(s, (str + sizeof(str)) - s);

}

const char *Atom_new(const char *str, int len)
{
    unsigned long h;
    int i;
    struct atom *p;

    assert(str);
    assert(len >= 0);

    /* str mapped to unsigned long */
    for (h = 0, i = 0; i < len; i++)
        h = (h << 1) + scatter[(unsigned char)str[i]];
    h %= NELEMS(buckets);
    /* atom is already in bucket */
    for (p = buckets[h]; p; p = p->link)
        if (len == p->len) {
            for (i = 0; i < len && p->str[i] == str[i]; )
                i++;
            if (i == len)
                return p->str;
        }
    /* allocate memory for a new atom */
    p = ALLOC(sizeof(*p) + len + 1);
    p->len = len;
    p->str = (char *)(p + 1);
    if (len > 0)
        memcpy(p->str, str, len);
    p->str[len] = '\0';
    p->link = buckets[h];
    buckets[h] = p;

    return p->str;
}

int Atom_length(const char *str)
{
    struct atom *p;
    int i;

    assert(str);
    for (i = 0; i < NELEMS(buckets); i++)
        for (p = buckets[i]; p; p = p->link)
            if (p->str == str)
                return p->len;

    /* shouldn't be here */
    assert(0);
    return 0;
}

void Atom_free(const char *str)
{
    struct atom *p, *q;
    int i;

    assert(str);
    for (i = 0; i < NELEMS(buckets); i++)
        for (p = buckets[i], q = buckets[i]; p; q = p, p = p->link) {
            if (p->str == str) {
                q->link = p->link;
                FREE(p);
            }
        }

    /* shouldn't be here */
    assert(0);
}

void Atom_reset(void)
{
    struct atom *p;
    int i;

    for (i = 0; i < NELEMS(buckets); i++) {
        while (buckets[i] != NULL) {
            p = buckets[i];
            buckets[i] = p->link;
            FREE(p);
        }
    }
}

void Atom_vload(const char *str, ...)
{
    va_list ap; /* points to each unnamed arg in turn */
    char *s;

    va_start(ap, str);
    while ((s = va_arg(ap, char *)) != NULL)
        Atom_string(s);

    va_end(ap); /* clean up when done */
}

void Atom_aload(const char *str[])
{
    int i;

    i = 0;
    while(1) {
        if (str[i] == NULL)
            break;
        Atom_string(str[i]);
    }
}

/* similar to Atom_new implementation */
const char *Atom_add(const char *str, int len)
{
    unsigned long h;
    int i;
    struct atom *p;

    assert(str);
    assert(len >= 0);

    /* str mapped to unsigned long */
    for (h = 0, i = 0; i < len; i++)
        h = (h << 1) + scatter[(unsigned char)str[i]];
    h %= NELEMS(buckets);
    /* atom is already in bucket */
    for (p = buckets[h]; p; p = p->link)
        if (len == p->len) {
            for (i = 0; i < len && p->str[i] == str[i]; )
                i++;
            if (i == len)
                return p->str;
        }
    /* allocate memory for a new atom */
    p = ALLOC(sizeof(*p) + len + 1);
    p->len = len;
    p->str = str; /* don't need to copy characters if clients do not free pointer to strings */
    p->link = buckets[h];
    buckets[h] = p;
}
