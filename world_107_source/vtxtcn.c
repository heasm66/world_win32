#include <stdio.h>
#include <io.h>
#include <stdlib.h>
/* World C Version 1.07 copyright 1992 J.D.McDonald
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */
void            dump_buf(void);
long            z, zbig, zsmall;
long            htext[2000];
int             gtext[5];
char            chrbuf[90];
FILE           *vtext_dat, *qtext_inc, *objdes_inc;
FILE           *gtext_inc;
FILE           *q1text_dat;
short           packch;
int             i, u, kk, nold, size, number, nnmax, kq;
short           buffer[512];
int             bi;
main()
{

    vtext_dat = fopen("vtext.dat", "r");
    q1text_dat = fopen("q1text.dat", "wb");
    qtext_inc = fopen("qtext.inc", "w");
    objdes_inc = fopen("objdes.inc", "w");

    fprintf(objdes_inc, " short odistb[] = { 0 \n");
    for (i = 0; i < 2000; i++)
	htext[i] = 0;
    z = 0;
    zbig = 0;
    zsmall = 0;
    u = 0;
    nold = 0;
    bi = 0;

    do {
	fgets(chrbuf, 80, vtext_dat);
	for (i = 0; i < 85; i++) {
	    if (chrbuf[i] == '\012' || chrbuf[i] == '\015' || chrbuf[i]
		== '\0')
		break;
	}
	size = i;
	for (i = size - 1; i > 3; i--) {
	    if (chrbuf[i] != ' ')
		break;
	}
	size = i + 1;
	if (size < 9) {
	    chrbuf[8] = ' ';
	    size = 9;
	}
	number = atoi(chrbuf);

	if (number != nold)
	    u += 1;
	if (number < 1001)
	    gtext[1] = u;
	if (number < 2001)
	    gtext[2] = u;
	if (number < 3001)
	    gtext[3] = u;
	if (number < 9999)
	    gtext[4] = u;
	if (number > 3000 && number < 3999) {
	    fprintf(objdes_inc, "  , %6d \n", u);
	    nnmax = number - 3000;
	} else if (number > 5000 && number < 5999) {
	    fprintf(objdes_inc, "   , %6d \n", u);
	} else;
	if (number != nold)
	    htext[u] = z;
	nold = number;
	chrbuf[size] = '{';
	for (i = size + 1; i < 90; i++)
	    chrbuf[i] = '}';
	for (i = 8; i < size; i++) {
	    if (chrbuf[i] == ' ')
		chrbuf[i] = '`';
	    if (chrbuf[i] == '.')
		chrbuf[i] = '|';
	}
	kk = 8;
	while (1) {
	    if (bi == 512)
		dump_buf();

	    if (chrbuf[kk] < '`' || chrbuf[kk + 1]
		< '`' || chrbuf[kk + 2] < '`') {
		packch = -(chrbuf[kk] + (chrbuf[kk + 1] << 8));
		buffer[bi] = packch;
		kk += 2;
		zbig++;
		bi++;
		z++;
		if (chrbuf[kk - 1] == '{' || chrbuf[kk - 2] == '{')
		    break;
	    } else {
		packch = (chrbuf[kk + 2] - 96) * 1024 + (chrbuf[kk + 1] - 96)
		    * 32 + chrbuf[kk] - 96;
		if ((packch & 0377) == 10 || ((packch >> 8) & 0377) == 10) {
		    packch = -(chrbuf[kk] + (chrbuf[kk + 1] << 8));
		    buffer[bi] = packch;
		    kk += 2;
		    zbig++;
		    bi++;
		    z++;
		    if (chrbuf[kk - 1] == '{' || chrbuf[kk - 2] == '{')
			break;
		} else {
		    buffer[bi] = packch;
		    kk += 3;
		    zsmall++;
		    bi++;
		    z++;
		    if (chrbuf[kk - 1] == '{' || chrbuf[kk - 2] == '{' ||
			chrbuf[kk - 3] == '{')
			break;
		}
	    }
	}
    } while (number != 9999);
    dump_buf();
    fprintf(objdes_inc, "  } ; \n");
    u = ((u + 2) / 3) * 3;
    fprintf(qtext_inc, "#define RTSIZE %6d \n", u + 1);
    fprintf(qtext_inc, " unsigned short rtext[] = { 0 \n");

    for (kk = 1; kk <= u / 3; kk++) {
	kq = (kk - 1) * 3;
	fprintf(qtext_inc, " , %5ld, %5ld, %5ld \n", htext[kq + 1],
		htext[kq + 2], htext[kq + 3]);
    }
    fprintf(qtext_inc, "  }; \n");

    fclose(vtext_dat);
    fclose(q1text_dat);
    fclose(qtext_inc);
    fclose(objdes_inc);
    gtext_inc = fopen("gtext.inc", "w");
    fprintf(gtext_inc, "  int gtext[5] = { 0, %6d, %6d, %6d, %6d };\n"
	    ,gtext[1], gtext[2], gtext[3], gtext[4]);
    fclose(gtext_inc);
    printf(" packed: %8ld unpacked: %8ld \n", zsmall, zbig);
    return(0);
}

void dump_buf(void)
{
    fwrite(buffer, sizeof(short), 512, q1text_dat);
    bi = 0;
}
