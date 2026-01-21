#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define SUB 5

struct Student
{
    char rollnumber[20];
    char name[50];
    int marks[SUB];
    int sum;
    float percent;
    float cgpa;
    char Grade[3];   
    int isPass;
};

int checkRollnumber(char roll[], struct Student s[], int totalStudents);
int checkName(char name[]);
void calculate(struct Student *s);
void getGrade(float p, int pass, char Grade[]);
float getCGPA(char Grade[]);
void display(struct Student s[], int totalStudents);
void statistics(struct Student s[], int totalStudents);

int main()
{
    FILE *fp;
    struct Student students[MAX];
    int totalStudents, i, j;

    fp = fopen("input.txt", "r");
    if (fp == NULL){
        printf("File is not found ! Some error\n");
        return 1;
    }

    fscanf(fp, "%d", &totalStudents);

    for (i = 0; i < totalStudents; i++){
        fscanf(fp, "%s %s", students[i].rollnumber, students[i].name);
        if (!checkRollnumber(students[i].rollnumber, students, i)){
            printf("Invalid rollnumber\n");
            return 1;
        }
         if (!checkName(students[i].name)) {
            printf("Invalid name\n");
            return 1;
        }

        students[i].isPass = 1;

        for (j = 0; j < SUB; j++){
            fscanf(fp, "%d", &students[i].marks[j]);
            if (students[i].marks[j] < 0 || students[i].marks[j] > 100){
                printf("Invalid marks \n");
                return 1;
            }

            if (students[i].marks[j] < 50)
                students[i].isPass = 0;
        }

        calculate(&students[i]);
    }

    fclose(fp);

    display(students, totalStudents);
    statistics(students, totalStudents);

    printf("\n--- End of Result ---\n");
    return 0;
}

/* validate roll number */
int checkRollnumber(char rollnumber[], struct Student stu[], int totalStudents){
    int i;
    for (i = 0; rollnumber[i] != '\0'; i++){
        if (!isalnum(rollnumber[i])){
         return 0;
        }
    }
    for (i = 0; i < totalStudents; i++){
        if (strcmp(rollnumber, stu[i].rollnumber) == 0){
            return 0;
        } 
    }
    return 1;
}

/* validate name */
int checkName(char student_name[]){
    int i;
    for (i = 0; student_name[i] != '\0'; i++){
        if (!isalpha(student_name[i])){
            return 0;
        }
    }
    return 1;
}

/* calculate total, percentage, Grade and cgpa */
void calculate(struct Student *s){
    int i;
    s->sum = 0;
    for (i = 0; i < SUB; i++){
        s->sum += s->marks[i];
    }

    s->percent = s->sum / (float)SUB;
    getGrade(s->percent, s->isPass, s->Grade);
    s->cgpa = getCGPA(s->Grade);
}

/* Grade as per PDF using strings */
void getGrade(float p, int pass, char Grade[]){
    if (!pass) {
        strcpy(Grade, "F");
        return;
    }

    if (p >= 90) strcpy(Grade, "O");
    else if (p >= 85) strcpy(Grade, "A+");
    else if (p >= 75) strcpy(Grade, "A");
    else if (p >= 65) strcpy(Grade, "B+");
    else if (p >= 60) strcpy(Grade, "B");
    else if (p >= 55) strcpy(Grade, "C");
    else if (p >= 50) strcpy(Grade, "D");
    else strcpy(Grade, "F");
}

/* CGPA mapping */
float getCGPA(char Grade[])
{
    if (strcmp(Grade, "O") == 0) return 10.0;
    if (strcmp(Grade, "A+") == 0) return 9.0;
    if (strcmp(Grade, "A") == 0) return 8.0;
    if (strcmp(Grade, "B+") == 0) return 7.0;
    if (strcmp(Grade, "B") == 0) return 6.0;
    if (strcmp(Grade, "C") == 0) return 5.0;
    if (strcmp(Grade, "D") == 0) return 4.0;
    return 0.0;
}

/* display output */
void display(struct Student s[], int totalStudents)
{
    int i, j;

    printf("\nRoll No\tName\tMarks\t\t\tTotal\t%%\tGrade\tCGPA\n");
    printf("-----------------------------------------------------------------------------\n");

    for (i = 0; i < totalStudents; i++){
        printf("%s\t%s\t", s[i].rollnumber, s[i].name);

        for (j = 0; j < SUB; j++){
            printf("%d ", s[i].marks[j]);
        }

        printf("\t%d\t%.2f\t%s\t%.1f\n",
               s[i].sum, s[i].percent, s[i].Grade, s[i].cgpa);
    }
}

/* class statistics */
void statistics(struct Student s[], int totalStudents){
    int i;
    float sum = 0, high = s[0].percent, low = s[0].percent;
    int GradeCount[8] = {0};

    for (i = 0; i < totalStudents; i++){

        sum += s[i].percent;
        if (s[i].percent > high) {
            high = s[i].percent;
        }
        if (s[i].percent < low)  {
            low = s[i].percent;
        }
        if (strcmp(s[i].Grade, "O") == 0){
            GradeCount[0]++;
        }
        else if (strcmp(s[i].Grade, "A+") == 0){ 
            GradeCount[1]++;
        }
        else if (strcmp(s[i].Grade, "A") == 0){
             GradeCount[2]++;
        }
        else if (strcmp(s[i].Grade, "B+") == 0){
             GradeCount[3]++;
        }
        else if (strcmp(s[i].Grade, "B") == 0){
             GradeCount[4]++;
        }
        else if (strcmp(s[i].Grade, "C") == 0) {
            GradeCount[5]++;
        }
        else if (strcmp(s[i].Grade, "D") == 0) {
            GradeCount[6]++;
        }
        {
            GradeCount[7]++;
        }
    }

    printf("\nClass Average Marks : %.2f\n", sum / totalStudents);
    printf("Highest Percentage in class: %.2f\n", high);
    printf("Lowest Percentage in class : %.2f\n", low);
    printf("\nGrade Count:\n");
    printf("O : %d\nA+: %d\nA : %d\nB+: %d\nB : %d\nC : %d\nD : %d\nF : %d\n",
           GradeCount[0], GradeCount[1], GradeCount[2], GradeCount[3],
           GradeCount[4], GradeCount[5], GradeCount[6], GradeCount[7]);
}
