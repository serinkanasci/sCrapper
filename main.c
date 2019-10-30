#include <stdio.h>
#include <stdlib.h>
#include <curl.h>
#include <string.h>
#include <time.h>


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void automateTask(int hour, int minute, int second,CURL *curl, char* url)
{
    clock_t start, end;
    int tmp = 0;
    int sec = second*1000;
    sec += minute*(60*1000);
    sec += hour*(3600*1000);

    while(sec > 0)
    {
        start = clock();
        writeInFile(curl,url);
        Sleep(sec);
        end = clock();
        tmp = end - start;
        printf("%d",tmp);
    }
}

void writeInFile(CURL *curl, char *url)
{
    curl_easy_init();
    CURLcode res;
    FILE *dest;
    char errbuf[CURL_ERROR_SIZE];
    char outfilename[FILENAME_MAX] = "C:\\Users\\Serinkan\\Desktop\\Cours ESGI\\projet\\scraper\\code_source.txt";
    if(curl)
    {
        dest = fopen(outfilename,"a+");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, dest);
        res = curl_easy_perform(curl);
        fclose(dest);
        printf("%s \n", url);
    }
    if(res != CURLE_OK)
    {
        size_t len = strlen(errbuf);
        fprintf(stderr, "\nlibcurl: (%d) ", res);
        if(len)
        {
            fprintf(stderr, "%s%s", errbuf,((errbuf[len - 1] != '\n') ? "\n" : ""));
        }
        else
        {
            fprintf(stderr, "%s\n", curl_easy_strerror(res));
        }
    }
    fclose(dest);

}

int main(void)
{

    char test;
    int cmp_url = 0;
    int len = 0, nb_url = 0;
    char* url;
    FILE *fichier;
    CURL *curl;


    fichier = fopen("C:\\Users\\Serinkan\\Desktop\\Cours ESGI\\projet\\scraper\\config.txt","r");
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\Serinkan\\Desktop\\Cours ESGI\\projet\\scraper\\cacert.pem");

    if(fichier == NULL)
    {
        printf("Impossible de créer le fichier.\n");
        exit(EXIT_FAILURE);
    }

    url = malloc(sizeof(char) * 100);
    test = fgetc(fichier);

    while(!feof(fichier))
    {
        if(test == '=')
        {
            fseek(fichier, 1, SEEK_CUR);
            test = fgetc(fichier);
        }
        if(test == '>')
        {
            fseek(fichier, 1, SEEK_CUR);
            cmp_url ++;
            if(cmp_url % 2 == 0)
            {
                test = fgetc(fichier);
                while(test != '}')
                {
                    url[len] = test;
                    len++;
                    fseek(fichier, 0, SEEK_CUR);
                    test = fgetc(fichier);
                }
                nb_url +=1;
                url[len] = '\0';
                automateTask(0,0,10,curl, url);
                len = 0;
                cmp_url = 0;
            }
        }
        fseek(fichier, 0, SEEK_CUR);
        test = fgetc(fichier);
    }

    curl_easy_cleanup(curl);
    free(url);
    fclose(fichier);
    return 0;
}
