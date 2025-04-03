#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<time.h>

void init_table(int* shift_table, int n)
{
    for(int i = 0; i < 26; i++) {
        shift_table[i] = n;
    }
}

void preprocess(int* shift_table, char* pattern)
{
    int pattern_len = strlen(pattern);
    
    for(int i = 0; i < pattern_len - 1; i++) {
        shift_table[pattern[i] - 'a'] = pattern_len - 1 - i;
    }
}

int string_match(int* shift_table, char* pattern, char* text, FILE* output_file)
{
    int pattern_len = strlen(pattern);
    int text_len = strlen(text);
    int matches = 0;
    int star_pos = pattern_len - 1;  
    int cmp = 0;  
    int occurance = 0;  

    fprintf(output_file,"Occurrences:");
    
    while(star_pos < text_len)
    {
        matches = 0;
        for(int i = 0; i < pattern_len; i++) {
            int text_idx = star_pos - i;
            int pattern_idx = pattern_len - 1 - i;
            
            cmp++;
            
            if(text[text_idx] == pattern[pattern_idx]) {
                matches++;
            } else {
                break;  
            }
        }
        
        if(matches == strlen(pattern))
        {
            int index = star_pos - pattern_len + 1;
            fprintf(output_file,"%d,", index);
            occurance++;
        }
        
        int shift;
        if(star_pos + 1 < text_len) {
            char next_char = text[star_pos + 1];
            if(next_char >= 'a' && next_char <= 'z') {
                shift = shift_table[next_char - 'a'];
            } else {
                shift = pattern_len;  
            }
        } else {
            break;  
        }
        
        star_pos += shift;
    }
    
    fprintf(output_file,"\n");
    fprintf(output_file,"Comparisons:%d\n\n", cmp);
    return cmp;
}

void print_table(int* shift_table, FILE* output_file)
{
    fprintf(output_file,"BCST:\n");
    for(int i = 0; i < 26; i++)
    {
        fprintf(output_file,"%c:%d", (char)(i+97), shift_table[i]);
        if(i < 25) fprintf(output_file, ", ");
    }
    fprintf(output_file, "\n");
}

void testcase(FILE* values_file, FILE* input_file, FILE* output_file)
{
    char text[2000];
    char pattern[100];
    fscanf(input_file,"%s",text);
    fscanf(input_file,"%s",pattern);

    int* shift_table = calloc(26, sizeof(int));
    init_table(shift_table, strlen(pattern));
    preprocess(shift_table, pattern);
    print_table(shift_table, output_file);
    clock_t start = clock();
    int cmp = string_match(shift_table, pattern, text, output_file);
    clock_t end = clock();
    int elapse = (int)(((double)(end-start))/CLOCKS_PER_SEC*1000000000); 
    fprintf(values_file,"%ld,%ld,%d,%d\n", strlen(pattern), strlen(text), cmp, elapse);
    free(shift_table);
}

int main()
{	
    FILE *input_file = fopen("input.txt", "r");
    FILE *output_file = fopen("horspool_output_PES2UG23CS090.txt", "w");
    FILE *values_file = fopen("horspool_values.txt", "w");

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
        testcase(values_file, input_file, output_file);
        count += 1;
    }
    fclose(input_file);
    fclose(output_file);
    fclose(values_file);
    return 0;
}
