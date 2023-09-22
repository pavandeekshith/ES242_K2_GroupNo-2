#include "test.h"

#include <string.h> // for testing generate_splits()

/*
 * Generate k-selections of a[0..n-1] in lexicographic order and call process_selection to process them.
 *
 * The array b[] will have enough space to hold k elements.
 * For a selection i1, ..., ik, you should set b[0] = a[i1], ..., b[k-1] = a[ik].
 * Selections should be generated in lexicographic order.
 * a[0..k-1] is the smallest selection and a[n-k..n-1] is the largest.
 */
void client(int a[],int k,int ind[],int b[]){
    for(int i=0;i<k;i++){
        b[i]=a[ind[i]];
        //printf("%d ",a[ind[i]]);
    }
}

void perm(int a[],int k,int ind[],int start,int ci,int n,int b[],void *data,void (*process_selection)(int *b, int k, void *data)){
    if(ci==k){
        client(a,k,ind,b);
        process_selection(b, k, data);
        return;
    }
        
    for(int i=start;i<n;i++){
        ind[ci]=i;
        perm(a,k,ind,i+1,ci+1,n,b,data,process_selection);
    }

}



void generate_selections(int a[], int n, int k, int b[], void *data, void (*process_selection)(int *b, int k, void *data))
{
    int ind[k];
    int start=0;
    int ci=0;
    perm(a,k,ind,start,ci,n,b,data,process_selection);
}



/*
 * See Exercise 2 (a), page 94 in Jeff Erickson's textbook.
 * The exercise only asks you to count the possible splits.
 * In this assignment, you have to generate all possible splits into buf[]
 * and call process_split() to process them.
 * The dictionary parameter is an array of words, sorted in dictionary order.
 * nwords is the number of words in this dictionary.
 */
void generate_splits_helper(const char *source, const char *dictionary[], int nwords, char buf[], int buf_index, int source_index, void *data, void (*process_split)(char buf[], void *data)) {
    int n = strlen(source);
    // base case
    if (source_index == n) {
        if (buf[buf_index-1]!=' '){
            buf[buf_index] = '\0';
        }
        else buf[buf_index-1] = '\0';
        process_split(buf, data);
        return;
    }
    for (int i = source_index; i < n; i++) {

        int word_length = i - source_index + 1;
        char word[word_length + 1];
        strncpy(word, source + source_index, word_length);
        word[word_length] = '\0';
        
        // check if the word is in dictionary
        int is_word = 0;
        for (int j = 0; j < nwords; j++) {
            if (strcmp(word, dictionary[j]) == 0 && strcmp(word, source) != 0) {
                is_word = 1;
                break;
            }
        }

        if (is_word) {
            strcpy(buf + buf_index, word);
            buf_index += word_length;
            if (buf_index > 0) {
                buf[buf_index] = ' ';
                buf_index++;
            }
            // if the word of s from source_index till index i is a valid word call the function from source_index i+1
            // recurrsive step
            generate_splits_helper(source, dictionary, nwords, buf, buf_index, i + 1, data, process_split);
            // to backtrack remove it from buf
            buf_index -= (word_length + 1);
        }
    }
}

void generate_splits(const char *source, const char *dictionary[], int nwords, char buf[], void *data, void (*process_split)(char buf[], void *data)) {
    // use helper function
    generate_splits_helper(source, dictionary, nwords, buf, 0, 0, data, process_split);
}

/*
 * Transform a[] so that it becomes the previous permutation of the elements in it.
 * If a[] is the first permutation, leave it unchanged.
 */
void previous_permutation(int a[], int n)
{
    int i = n - 2;

    while (i >= 0 && a[i] <= a[i + 1]) {
        i--;
    }

    if (i < 0) {
        return ;
    }
    
    int j = n - 1;
    while (a[j] >= a[i]) {
        j--;
    }

    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;

    // Reverse the elements from i+1 to the end
    int start = i + 1;
    int end = n - 1;
    while (start < end) {
        temp = a[start];
        a[start] = a[end];
        a[end] = temp;
        start++;
        end--;
    }
}
/* Write your tests here. Use the previous assignment for reference. */

typedef struct {
    int index;
    int err;
} state_t;

static void test_selections_2165(int b[], int k, void *data)
{
    state_t *s = (state_t *)data;
    s->err = 0;
    switch (s->index) {
    case 0:
        if ((b[0] != 2) || (b[1] != 1)) {
            s->err = 1;
        }
        break;
    case 1:
        if ((b[0] != 2) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 2:
        if ((b[0] != 2) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 3:
        if ((b[0] != 1) || (b[1] != 6)) {
            s->err = 1;
        }
        break;
    case 4:
        if ((b[0] != 1) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    case 5:
        if ((b[0] != 6) || (b[1] != 5)) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
    ++(s->index);
}

BEGIN_TEST(generate_selections) {
    int a[] = { 2, 1, 6, 5 };
    int b[2];
    state_t s2165 = { .index = 0, .err = 1 };
    generate_selections(a, 4, 2, b, &s2165, test_selections_2165);
    ASSERT(!s2165.err, "Failed on 2 1 6 5.");
} END_TEST

void test_splits_art(char buf[], void *data)
{
    state_t *s = (state_t*)data;
    s->err = 0;
    switch (s->index) {
    case 0:
        if (!strcmp(buf, "art is toil")) {
            s->err = 1;
        }
        break;
    case 1:
        if (!strcmp(buf, "artist oil")) {
            s->err = 1;
        }
        break;
    default:
        s->err = 1;
    }
}

BEGIN_TEST(generate_splits) {
    const char *a = "artistoil";
    const char *dict[] = {
        "art",
        "artist",
        "is",
        "oil",
        "toil"
    };
    int nwords = 5;
    state_t s = { .index = 0, .err = 1 };
    char buf[256];
    generate_splits(a, dict, nwords, buf, &s, test_splits_art);
    ASSERT(!s.err, "Failed on 'artistoil'.");
} END_TEST

BEGIN_TEST(previous_permutation) {
    int a[] = { 1, 5, 6, 2, 3, 4 };
    previous_permutation(a, 6);
    ASSERT_ARRAY_VALUES_EQ(a, 6, "Failed on 1 5 6 2 3 4.", 1, 5, 4, 6, 3, 2);
} END_TEST

int main()
{
    run_tests((test_t[]) {
            TEST(generate_selections),
            TEST(generate_splits),
            TEST(previous_permutation),
            0
        });
    return 0;
}
