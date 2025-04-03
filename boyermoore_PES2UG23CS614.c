#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int* bcst_create(const char* pattern, int pattern_len) {
    int* bcst = (int*)malloc(26 * sizeof(int));
    if (bcst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < 26; i++) {
        bcst[i] = pattern_len;
    }
    
    for (int i = 0; i < pattern_len - 1; i++) {
        char c = pattern[i];
        if (c >= 'a' && c <= 'z') {
            bcst[c - 'a'] = pattern_len - 1 - i;
        }
    }
    
    return bcst;
}

int* gsst_create(const char* pattern, int pattern_len) {
    int* gsst = (int*)malloc((pattern_len + 1) * sizeof(int));
    if (gsst == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int x = 1; x <= pattern_len; x++) {
        int r2 = 0;
        char* suffix = (char*)malloc((x + 1) * sizeof(char));
        if (suffix == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
		for(int i=0;i<x;i++)
		{
			suffix[x-i-1] = pattern[pattern_len-i-1];
		}
		suffix[x] = '\0';

		char mis_char = (pattern_len - x - 1 >= 0) ? pattern[pattern_len - x - 1] : '~';

        char* rev_pattern = (char*)malloc(pattern_len * sizeof(char));
        if (rev_pattern == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<pattern_len-1;i++)
			rev_pattern[i] = pattern[pattern_len-i-2];
        rev_pattern[pattern_len-1] = '\0';

        char* rev_suffix = (char*)malloc((x + 1) * sizeof(char));
        if (rev_suffix == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<x;i++)
			rev_suffix[i] = suffix[x-i-1];
        rev_suffix[x] = '\0';

        int count = 0;
        while (1) {
            char* pos_ptr = strstr(rev_pattern, rev_suffix);
            if (pos_ptr == NULL) {
                r2 = 1;
                break;
            }
            int pos = pos_ptr - rev_pattern;
            char check_char = (pos + x < pattern_len - 1) ? rev_pattern[pos + x] : '`';
            if (check_char != mis_char) {
                gsst[x] = pos + count + 1;
                break;
            }
            rev_pattern += pos + 1;
            count += pos + 1;
        }

		char* suffix_ptr = suffix;
        if (r2) {
            char* prefix = (char*)malloc(pattern_len * sizeof(char));
            if (prefix == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < pattern_len; i++) {
                prefix[i] = pattern[i];
            }
            prefix[pattern_len] = '\0';
            while (1) {
                if (strlen(suffix) == 1) {
                    gsst[x] = pattern_len;
                    break;
                }
                suffix++;
                if (strncmp(prefix, suffix, strlen(suffix)) == 0) {
                    gsst[x] = pattern_len - strlen(suffix);
                    break;
                }
            }
        }
    }
    return gsst;
}

int boyer_moore(const char* text, const char* pattern, int* bcst, int* gsst, FILE* output_file) 
{
    int pattern_len = strlen(pattern);
    int text_len = strlen(text);
    int comparisons = 0;
    int pos = pattern_len - 1; 
    
    fprintf(output_file, "Occurrences:");
    
    while (pos < text_len) {
        int end = pos;
        int i = pattern_len - 1;
        int match = 0;
        
        while (i >= 0 && text[end] == pattern[i]) {
            comparisons++;
            match++;
            end--;
            i--;
        }
        
        if (i < 0) {
            fprintf(output_file, "%d,", end + 1);
            pos += 1;
        } else {
            comparisons++;
            
            char mismatched_char = text[end];
            
            int bc_shift = 0;
            if (mismatched_char >= 'a' && mismatched_char <= 'z') {
                bc_shift = bcst[mismatched_char - 'a'] - (pattern_len - 1 - i);
            } else {
                bc_shift = pattern_len;
            }
            
            int gs_shift = gsst[pattern_len - match];
            
            int shift = (bc_shift > gs_shift) ? bc_shift : gs_shift;
            
            if (shift <= 0) shift = 1;
            
            pos += shift;
        }
    }
    
    fprintf(output_file, "\n");
    return comparisons;
}

void testcase(FILE* values_file, FILE* input_file, FILE* output_file)
{
	char text[2000];
    char pattern[500];
    fscanf(input_file,"%s",text);
    fscanf(input_file,"%s",pattern);


    int pattern_len = strlen(pattern);
    int* bcst = bcst_create(pattern, pattern_len);
    fprintf(output_file,"BCST:\n");
    for(int i = 0; i < 26; i++){
		fprintf(output_file,"%c:%d, ", (char)(i+'a'), bcst[i]);
    }
    fprintf(output_file,"\n");

    int* gsst = gsst_create(pattern, pattern_len);
    fprintf(output_file,"GSST:\n");
    for(int i = 1; i <= pattern_len; i++){
        fprintf(output_file,"%d:%d, ", i, gsst[i]);
    }
    fprintf(output_file,"\n");

    clock_t start = clock();
    int comparisons = boyer_moore(text, pattern, bcst, gsst, output_file);
    fprintf(output_file,"Comparisons:%d\n\n", comparisons);

    free(bcst);
    free(gsst);
	clock_t end = clock();
	int elapse=(int)(((double)(end-start))/CLOCKS_PER_SEC*1000000000); //seconds to nanoseconds
	fprintf(values_file,"%d,%ld,%d,%d\n",pattern_len,strlen(text),comparisons,elapse);
}

int main() {
	FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("boyermoore_output_PES2UG23CS090.txt", "w");
    FILE *values_file = fopen("boyermoore_values.txt", "w");

    if (!input_file || !output_file || !values_file) {
        printf("Error opening file!\n");
        return 1;
    }
    int testcases;
	fscanf(input_file,"%d",&testcases);
	int count = 0;
	fprintf(values_file, "patternlen,textlen,cmp,timetaken\n");
	while(count < testcases)
	{
		testcase(values_file,input_file, output_file);
		count += 1;
	}
	fclose(input_file);
	fclose(output_file);
	fclose(values_file);
    return 0;
}
