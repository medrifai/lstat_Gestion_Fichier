#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_file_type(mode_t mode) {
    if (S_ISREG(mode))
        printf("Type de fichier : Fichier régulier\n");
    else if (S_ISDIR(mode))
        printf("Type de fichier : Répertoire\n");
    else if (S_ISLNK(mode))
        printf("Type de fichier : Lien symbolique\n");
    else if (S_ISCHR(mode))
        printf("Type de fichier : Périphérique de caractère\n");
    else if (S_ISBLK(mode))
        printf("Type de fichier : Périphérique de bloc\n");
    else if (S_ISFIFO(mode))
        printf("Type de fichier : FIFO (tube nommé)\n");
    else if (S_ISSOCK(mode))
        printf("Type de fichier : Socket\n");
    else
        printf("Type de fichier : Inconnu\n");
}

void print_permissions(mode_t mode) {
    printf("Permissions : ");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <nom_du_fichier>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *file_name = argv[1];
    struct stat file_stat;

    // Utilisation de lstat
    if (lstat(file_name, &file_stat) == -1) {
        perror("Erreur avec lstat");
        return EXIT_FAILURE;
    }

    printf("Informations sur le fichier : %s\n", file_name);
    print_file_type(file_stat.st_mode);
    printf("Taille : %ld octets\n", file_stat.st_size);
    printf("Nombre de liens : %ld\n", file_stat.st_nlink);

    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    printf("Propriétaire : %s\n", pw ? pw->pw_name : "Inconnu");
    printf("Groupe : %s\n", gr ? gr->gr_name : "Inconnu");

    print_permissions(file_stat.st_mode);

    printf("Dernière modification : %s", ctime(&file_stat.st_mtime));

    return EXIT_SUCCESS;
}