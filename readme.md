# World, version 1.07 for Windows
Instructions on how to build World for Windows. Project also contains already generated data files and binaries.
### Build with CygWin
1. Generate data files:  
    * `cc vtxtcn.c -o vtxtcn`
    * `./vtxtcn`
    * `cc vcnvrt.c -o vcnvrt`
    * `./vcnvrt`
2. Compile game:  
    * `cc world.c helper.c demons.c motion.c parser.c verbs1.c verbs2.c -o world`
3. To run: copy files `world.exe`, `q1text.dat`and `cygwin1.dll` (from `/cygwin64/bin/`) to your destination.

### Build with Visual Studio
1. Generate data files in CygWin per step 1 above.
2. Create project with the source files: `world.c`,	`helper.c`, `demons.c`, `motion.c`, `parser.c`, `verbs1.c` and `verbs2.c`.
3. Add the header files `arrays.h`and `variabs.h` to project.
4. Add all inc-files generated above (9 files) to project.
5. Add `_CRT_SECURE_NO_WARNINGS` to preprocessing.
6. Change function `vsuspe` in `helper.c` with the following (see full code below):
	* Set `buffer_size` to 200 instead of 256 because 256 makes `iret` return wrong number of items.
	* Change last `OBJMAX` to `MOVMAX` instead to remove warning about obloc overrun.
7. Compile for x86 release.
8. To run: copy files `world.exe` and `q1text.dat` to your destination.

### Playing
Navigate to `build` directory from console or explorer and run `world.exe`.  
Make sure `q1text.dat` is in the same directory

## Authors and acknowledgment
All game code is copyright 1992 J.D.McDonald

## License
World C is free for non-commercial purposes.

### Modified `vsuspe`
```
vsuspe(inout)
    int             inout;
{
    short            sbuffer[256];
    int             result, filedes, i, iret;
    short           *sbptr, *iptr;
    result = 0;

    if (inout != 1) {
    /* "suspend" or "save"  */

    filedes = creat(filenm, 0600);
    if (filedes == -1) {
        linout("I failed to create your save file.", 34);
        return (result);
    }
    sbptr = sbuffer;
    *sbptr++ = horflg;
    for (i = 0; i < 6; i++)
        *sbptr++ = wirelc[i];
    *sbptr++ = turns;
    *sbptr++ = loc;
    *sbptr++ = oldloc;
    *sbptr++ = oldlc2;
    *sbptr++ = brfflg;
    *sbptr++ = deadf;
    *sbptr++ = dirty;
    *sbptr++ = nonext;
    *sbptr++ = spcloc;
    *sbptr++ = fshlif;
    *sbptr++ = noshoe;
    *sbptr++ = daytim;
    *sbptr++ = martim;
    for (i = 0; i < 9; i++)
        *sbptr++ = marflg[i];
    *sbptr++ = oextim;
    *sbptr++ = rdietm;
    *sbptr++ = rvtim;
    *sbptr++ = gvtim;
    *sbptr++ = eattim;
    *sbptr++ = easttm;
    *sbptr++ = filmtm;
    *sbptr++ = screef;
    *sbptr++ = eastsc;
    *sbptr++ = cactsc;
    *sbptr++ = diesc;
    *sbptr++ = filmst;
    *sbptr++ = machst;
    *sbptr++ = dial1;
    *sbptr++ = dial2;
    *sbptr++ = dial1x;
    *sbptr++ = dial2x;
    *sbptr++ = bonus;
    *sbptr++ = jackpo;
    *sbptr++ = lpill;
    *sbptr++ = pbstat;
    *sbptr++ = decset;
    *sbptr++ = raset;
    *sbptr++ = dcombi;
    *sbptr++ = chaset;
    *sbptr++ = guardl;
        *sbptr++ = fimage;
    write(filedes, sbuffer, 64*sizeof(short));
    sbptr = sbuffer;
    iptr = locdat;
    for (i = 0; i <= LOCNUM; i++)
        *sbptr++ = *iptr++;
    write(filedes, sbuffer, 200*sizeof(short));
    sbptr = sbuffer;
    iptr = obimpr;
    for (i = 0; i <= OBJMAX; i++)
        *sbptr++ = *iptr++;
    write(filedes, sbuffer, 200*sizeof(short));
    sbptr = sbuffer;
    iptr = obloc;
    for (i = 0; i <= OBJMAX; i++)
        *sbptr++ = *iptr++;
    write(filedes, sbuffer, 200*sizeof(short));
    close(filedes);
    return (result);
    } else {
    /* "restore"    */

    filedes = open(filenm, 0);
    if (filedes == -1)
        goto lab1;
    iret = read(filedes, sbuffer, 64*sizeof(short));
    if (iret != 64*sizeof(short))
        goto lab1;
    sbptr = sbuffer;
    horflg = *sbptr++;
    for (i = 0; i < 6; i++)
        wirelc[i] = *sbptr++;
    turns = *sbptr++;
    loc = *sbptr++;
    oldloc = *sbptr++;
    oldlc2 = *sbptr++;
    brfflg = *sbptr++;
    deadf = *sbptr++;
    dirty = *sbptr++;
    nonext = *sbptr++;
    spcloc = *sbptr++;
    fshlif = *sbptr++;
    noshoe = *sbptr++;
    daytim = *sbptr++;
    martim = *sbptr++;
    for (i = 0; i < 9; i++)
        marflg[i] = *sbptr++;
    oextim = *sbptr++;
    rdietm = *sbptr++;
    rvtim = *sbptr++;
    gvtim = *sbptr++;
    eattim = *sbptr++;
    easttm = *sbptr++;
    filmtm = *sbptr++;
    screef = *sbptr++;
    eastsc = *sbptr++;
    cactsc = *sbptr++;
    diesc = *sbptr++;
    filmst = *sbptr++;
    machst = *sbptr++;
    dial1 = *sbptr++;
    dial2 = *sbptr++;
    dial1x = *sbptr++;
    dial2x = *sbptr++;
    bonus = *sbptr++;
    jackpo = *sbptr++;
    lpill = *sbptr++;
    pbstat = *sbptr++;
    decset = *sbptr++;
    raset = *sbptr++;
    dcombi = *sbptr++;
    chaset = *sbptr++;
    guardl = *sbptr++;
    fimage = *sbptr++;
    iret = read(filedes, sbuffer, 200*sizeof(short));
    if (iret != 200*sizeof(short))
        goto lab1;
    sbptr = sbuffer;
    iptr = locdat;
    for (i = 0; i <= LOCNUM; i++)
        *iptr++ = *sbptr++;
    iret = read(filedes, sbuffer, 200*sizeof(short));
    if (iret != 200*sizeof(short))
        goto lab1;
    sbptr = sbuffer;
    iptr = obimpr;
    for (i = 0; i <= OBJMAX; i++)
        *iptr++ = *sbptr++;
    iret = read(filedes, sbuffer, 200*sizeof(short));
    if (iret != 200*sizeof(short))
        goto lab1;
    sbptr = sbuffer;
    iptr = obloc;
    for (i = 0; i <= MOVMAX; i++)
        *iptr++ = *sbptr++;
        close(filedes);
    result = 1;
    chaser = 0;
    linout(" ", 1);
    return (result);
    }
lab1:
    linout("Restore failed.", 15);
    return (result);
}
```