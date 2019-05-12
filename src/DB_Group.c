#include "../lib/DB_Group.h"

/****************************************************************************************/
/*  I : Database to which import the groups CSV file                                    */
/*  P : Reads the whole groups CSV file and writes it in the Database file              */
/*  O : /                                                                               */
/****************************************************************************************/
void Import_CSV_Group(dbc *db){
    int i=0;
    char line[BUF_LEN];
    char fld[BUF_LEN];
    char *ptr1, *ptr2;
	cgrp grp;
	FILE *fpi, *fp_lg;

    db->fp = fopen(DB_file, "rb+");
    fp_lg = fopen(log_file, "a");

	fpi = fopen(CSV_grp_imp, "r");
	if (fpi == NULL) { printf("Erreur\n"); return; }

    printf("\nGroups : importing ...\n");

    fgets(line, 200, fpi);

    fseek(db->fp, db->hdr.off_grp, SEEK_SET);

    printf("%08lX\n",db->hdr.off_grp);

    while (fgets(line, 200, fpi) != NULL)
    {
        memset(&grp, 0, sizeof(cgrp));
        strcpy(grp.tp_rec, "GRP");

        if (PRT) printf("\n---------------------------\n%s\n",line);
        if (PRT) printf("%s\n", line);

        ptr1 = strtok(line,";");                   if (PRT) printf("%s\n", ptr1);
        ptr2 = strtok(NULL,";");                   if (PRT) printf("%s\n", ptr2);
        memset(fld, 0, BUF_LEN);
        strncpy(fld, ptr1, ptr2-ptr1-1);
        grp.id_grp = atoi(fld);                    if (PRT) printf("%d\n", grp.id_grp);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.nm_grp, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.nm_grp);
        ptr1 = ptr2;
        ptr2 = strtok(NULL,";");
        strncpy(grp.cd_cty, ptr1, ptr2-ptr1-1);    if (PRT) printf("%s\n", grp.cd_cty);
        ptr1 = ptr2;
        strncpy(fld, ptr1, strlen(ptr1)-1);
        grp.id_cty = atoi(fld);                    if (PRT) printf("%d\n", grp.id_cty);

        fwrite(&grp, 1, sizeof(cgrp), db->fp);

        i++;
    }

    db->nr_grp = i;

    fprintf(fp_lg, "Groups imported : %d \n", db->nr_grp);

    fclose(db->fp);
    fclose(fp_lg);
	fclose(fpi);

    printf("\nGroups imported : %d \n\n", db->nr_grp);

	return ;
}
