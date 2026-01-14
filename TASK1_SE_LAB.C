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
    char grade;
    int isPass;
};

int checkRollnumber(char roll[], struct Student s[], int totalStudents);
int checkName(char name[]);
void calculate(struct Student *s);
char getGrade(float p, int pass);
float getCGPA(char grade);
void display(struct Student s[], int totalStudents);
void statistics(struct Student s[], int totalStudents);

int main()
{
    FILE *fp;
    struct Student students[MAX];
    int totalStudents, i, j;

    fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    fscanf(fp, "%d", &totalStudents);

    for (i = 0; i < totalStudents; i++)
    {
        fscanf(fp, "%s %s", students[i].rollnumber, students[i].name);

        if (!checkRollnumber(students[i].rollnumber, students, i))
        {
            printf("Invalid or duplicate roll number\n");
            return 1;
        }

        if (!checkName(students[i].name))
        {
            printf("Invalid name\n");
            return 1;
        }

        students[i].isPass = 1;

        for (j = 0; j < SUB; j++)
        {
            fscanf(fp, "%d", &students[i].marks[j]);

            if (students[i].marks[j] < 0 || students[i].marks[j] > 100)
            {
                printf("Invalid marks\n");
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

    printf("\n--- End of Result  ---\n");

    return 0;
}

/* validate roll number */
int checkRollnumber(char roll[], struct Student s[], int totalStudents)
{
    int i, j;

    for (i = 0; roll[i] != '\0'; i++)
    {
        if (!isalnum(roll[i]))
            return 0;
    }

    for (j = 0; j < totalStudents; j++)
    {
        if (strcmp(roll, s[j].rollnumber) == 0)
            return 0;
    }

    return 1;
}

/* validate name */
int checkName(char name[])
{
    int i;
    for (i = 0; name[i] != '\0'; i++)
    {
        if (!isalpha(name[i]))
            return 0;
    }
    return 1;
}

/* calculate sum, percentage, grade and cgpa */
void calculate(struct Student *s)
{
    int i;
    s->sum = 0;

    for (i = 0; i < SUB; i++)
        s->sum += s->marks[i];

    s->percent = s->sum / (float)SUB;
    s->grade = getGrade(s->percent, s->isPass);
    s->cgpa = getCGPA(s->grade);
}

/* grade as per PDF */
char getGrade(float p, int pass)
{
    if (!pass) return 'F';
    if (p >= 90) return 'O';
    else if (p >= 85) return 'P';   // A+
    else if (p >= 75) return 'A';
    else if (p >= 65) return 'Q';   // B+
    else if (p >= 60) return 'B';
    else if (p >= 55) return 'C';
    else if (p >= 50) return 'D';
    else return 'F';
}

/* CGPA mapping */
float getCGPA(char grade)
{
    switch (grade)
    {
        case 'O': return 10.0;
        case 'P': return 9.0;
        case 'A': return 8.0;
        case 'Q': return 7.0;
        case 'B': return 6.0;
        case 'C': return 5.0;
        case 'D': return 4.0;
        default:  return 0.0;
    }
}

/* display output */
void display(struct Student s[], int totalStudents)
{
    int i, j;

    printf("\nRoll No\t\tName\tMarks\t\t\tTotal\t%%\tGrade\tCGPA\n");
    printf("-----------------------------------------------------------------------------\n");

    for (i = 0; i < totalStudents; i++)
    {
        printf("%s\t%s\t", s[i].rollnumber, s[i].name);

        for (j = 0; j < SUB; j++)
            printf("%d ", s[i].marks[j]);

        printf("\t%d\t%.2f\t%c\t%.1f\n",
               s[i].sum, s[i].percent, s[i].grade, s[i].cgpa);
    }
}

/* class statistics */
void statistics(struct Student s[], int totalStudents)
{
    int i;
    float sum = 0, high, low;
    int gradeCount[8] = {0};

    high = low = s[0].percent;

    for (i = 0; i < totalStudents; i++)
    {
        sum += s[i].percent;

        if (s[i].percent > high) high = s[i].percent;
        if (s[i].percent < low)  low = s[i].percent;

        switch (s[i].grade)
        {
            case 'O': gradeCount[0]++; break;
            case 'P': gradeCount[1]++; break;
            case 'A': gradeCount[2]++; break;
            case 'Q': gradeCount[3]++; break;
            case 'B': gradeCount[4]++; break;
            case 'C': gradeCount[5]++; break;
            case 'D': gradeCount[6]++; break;
            case 'F': gradeCount[7]++; break;
        }
    }

    printf("\nClass Average: %.2f\n", sum / totalStudents);
    printf("Highest Percentage: %.2f\n", high);
    printf("Lowest Percentage : %.2f\n", low);

    printf("\nGrade Count:\n");
    printf("O : %d\nA+: %d\nA : %d\nB+: %d\nB : %d\nC : %d\nD : %d\nF : %d\n",
           gradeCount[0], gradeCount[1], gradeCount[2], gradeCount[3],
           gradeCount[4], gradeCount[5], gradeCount[6], gradeCount[7]);
}
