#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

struct seeker
{
    char fn[50];
    char ln[50];
    char un[50];
    char pw[50];
    long phn;
    char email[50];
};

struct recruiter
{
    char cmpn[50];
    char un[50];
    char pw[50];
    long phn;
    char email[50];
};

typedef struct seekerprofile
{
    char seeker_un[50];
    int age;
    char gen[10];
    char loc[100];
    int sslc;
    int hsc;
    char qual[10];
    float cgpa;
    float workex;
    char desig[100];
    char certi[100];
} seekerprofile;

typedef struct jobprofile
{
    char recruiter_un[50];
    char cmpn[50];
    char job[50];
    char qual[10];
    char skills[100];
    char loc[100];
    long sal;
    float workex;
    int Nightshift;
} jobprofile;

typedef struct seekerconstraint
{
    char seeker_un[50];
    long minsalary;
    char location[50];
    int Nightshift;
    int constraint_type[3];
} seekerconstraint;

typedef struct recruiterconstraint
{
    char recruiter_un[50];
    char job[50];
    float mincgpa;
    float minworkex;
    char quali[50];
    char location[100];
    int constraint_type[4];
}recruiterconstraint;

void register_seeker()
{
    struct seeker s;
    printf("SEEKER REGISTRATION: \n");
    printf("-------------------- \n");
    printf("Enter your first name: ");
    scanf("%s", s.fn);
    printf("Enter your last name: ");
    scanf("%s", s.ln);
    printf("Create a username for your account: ");
    scanf("%s", s.un);
    printf("Enter a password: ");
    scanf("%s", s.pw);
    printf("Enter your phone number: ");
    scanf("%ld", &s.phn);
    printf("Enter your email id: ");
    scanf("%s", s.email);

    FILE* file = fopen("seeker.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s %s %s %s %ld %s \n", s.fn, s.ln, s.un, s.pw, s.phn, s.email);
        fclose(file);
        printf("Seeker registration successful! \n");
    }
    else
    {
        printf("Error opening file. Please try again. \n");
    }
}

void register_recruiter()
{
    struct recruiter r;
    printf("RECRUITER REGISTRATION: \n");
    printf("----------------------- \n");
    printf("Enter company name: ");
    scanf("%s", r.cmpn);
    printf("Create a username for your account: ");
    scanf("%s", r.un);
    printf("Enter a password: ");
    scanf("%s", r.pw);
    printf("Enter your phone number: ");
    scanf("%ld", &r.phn);
    printf("Enter your email id: ");
    scanf("%s", r.email);

    FILE* file = fopen("recruiter.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%s %s %s %ld %s \n", r.cmpn, r.un, r.pw, r.phn, r.email);
        fclose(file);
        printf("Recruiter registration successful! \n");
    }
    else
    {
        printf("Error opening file. Please try again. \n");
    }
}

int authenticate(char* un, char* pw, int opt)
{
    FILE* file = NULL;
    char line[200], file_un[50], file_pw[50];
    if (opt == 1)
    {
        file = fopen("seeker.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%*s %*s %s %s %*ld %*s", file_un, file_pw);
            if (strcmp(un, file_un) == 0 && strcmp(pw, file_pw) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }
    else if (opt == 2)
    {
        file = fopen("recruiter.txt", "r");
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%*s %s %s %*ld %*s", file_un, file_pw);
            if (strcmp(un, file_un) == 0 && strcmp(pw, file_pw) == 0)
            {
                fclose(file);
                return 1;
            }
        }
    }

    if (file == NULL)
    {
        printf("Error opening file. Please try again. \n");
        return 0;
    }

    fclose(file);
    return 0;
}

int login(char un[])
{
    int opt;
    char pw[50];
    while (true)
    {
        printf("Are you a Job Seeker or a Recruiter? Enter 1 for Seeker and 2 for Recruiter: ");
        scanf("%d", &opt);
        if ((opt != 1) && (opt != 2))
        {
            printf("Please enter a valid option. \n \n");
            continue;
        }
        break;
    }
    printf("\n");
    printf("LOGIN: \n");
    printf("------------------------------- \n");
    printf("Enter username: ");
    scanf("%s", un);
    printf("Enter password: ");
    scanf("%s", pw);

    if (!authenticate(un, pw, opt))
    {
        printf("Invalid username or password. Please try again. \n");
        return 0;
    }
    printf("Login successful! \n\n");
    return opt;
}

int readSeekerConstraints(seekerconstraint constraints[], const char* filename, const char* seekerUsername)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file.Please try again. \n");
        return 0;
    }

    int constraintCount = 0;
    char line[200];
    while (fgets(line, sizeof(line), file))
    {
        seekerconstraint currentConstraint;
        sscanf(line, "%s %ld %s %d %d %d %d",
            currentConstraint.seeker_un,
            &currentConstraint.minsalary,
            currentConstraint.location,
            &currentConstraint.Nightshift,
            &currentConstraint.constraint_type[0],
            &currentConstraint.constraint_type[1],
            &currentConstraint.constraint_type[2]);

        // Assuming a maximum of 3 constraint types
        // Check if the seeker username matches
         // constraintCount is incremented ONLY when the username in the file 
             //record matches the current username
        if (strcmp(currentConstraint.seeker_un, seekerUsername) == 0)
        {
            constraints[constraintCount] = currentConstraint;
            constraintCount++;
        }
    }
    fclose(file);
    //Returns 1 more than the index of the "constraints" array where the last element
    //of the array contains the MOST RECENT SEEKER CONSTRAINT values
    return constraintCount;
}

int readRecruiterConstraints(recruiterconstraint constraints[], const char* filename, const char* recruiterUsername, const char* JobTitle)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file. Please try again. \n");
        return 0;
    }

    int constraintCount = 0;
    char line[200];

    while (fgets(line, sizeof(line), file))
    {
        recruiterconstraint currentConstraint;
        sscanf(line, "%s %s %f %f %s %s %d %d %d %d",
            currentConstraint.recruiter_un,
            currentConstraint.job,
            &currentConstraint.mincgpa,
            &currentConstraint.minworkex,
            currentConstraint.quali,
            currentConstraint.location,
            &currentConstraint.constraint_type[0],
            &currentConstraint.constraint_type[1],
            &currentConstraint.constraint_type[2],
            &currentConstraint.constraint_type[3]);

        // Assuming a maximum of 4 constraint types
        // Check if the recruiter username and recruiter job title matches with the entered username and job title
        // constraintCount is incremented ONLY when the username in the file 
        //record matches the current username
        if ((strcmp(currentConstraint.recruiter_un, recruiterUsername) == 0) && (strcmp(currentConstraint.job, JobTitle) == 0))
        {
            constraints[constraintCount] = currentConstraint;
            constraintCount++;
        }
    }
    fclose(file);
    //Returns 1 more than the index of the "constraints" array where the last element
    //of the array contains the MOST RECENT SEEKER CONSTRAINT values
    return constraintCount;
}

int readSeekerProfiles(seekerprofile profiles[], const char* filename, char un[])
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file. Please try again. \n");
        return 0;
    }

    int profileCount = 0;
    char line[500];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%s %d %s %s %d %d %s %f %f %s %s",
            profiles[profileCount].seeker_un,
            &profiles[profileCount].age,
            &profiles[profileCount].gen,
            profiles[profileCount].loc,
            &profiles[profileCount].sslc,
            &profiles[profileCount].hsc,
            profiles[profileCount].qual,
            &profiles[profileCount].cgpa,
            &profiles[profileCount].workex,
            profiles[profileCount].desig,
            profiles[profileCount].certi);
        // checking if username already exists in file
        if ((strcmp(un, profiles[profileCount].seeker_un) == 0) || (strcmp(un, "") == 0))
        {
            profileCount++;
        }
    }

    fclose(file);
    return profileCount;
}

// Function to read job profiles from a file
int readJobProfiles(jobprofile profiles[], const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file. Please try again. \n");
        return 0;
    }

    int profileCount = 0;
    char line[500];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%s %s %s %s %s %s %ld %f %d",
            profiles[profileCount].recruiter_un,
            profiles[profileCount].cmpn,
            profiles[profileCount].job,
            profiles[profileCount].qual,
            profiles[profileCount].skills,
            profiles[profileCount].loc,
            &profiles[profileCount].sal,
            &profiles[profileCount].workex,
            &profiles[profileCount].Nightshift);
        profileCount++;
    }

    fclose(file);
    return profileCount;
}

float calculateMatchPercentageforSeeker(jobprofile profile, seekerconstraint constraint)
{
    int score = 0;

    // Check constraint_type[0] for minsalary
    if (constraint.constraint_type[0] == 1)
    {
        if (profile.sal >= constraint.minsalary)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[0] == 2)
    {
        if (profile.sal >= constraint.minsalary)
            score += 2; // Mandatory weightage
    }

    // Check constraint_type[1] for location
    if (constraint.constraint_type[1] == 1)
    {
        if (strcmp(profile.loc, constraint.location) == 0)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[1] == 2)
    {
        if (strcmp(profile.loc, constraint.location) == 0)
            score += 2; // Mandatory weightage
    }

    // Check constraint_type[2] for Nightshift
    if (constraint.constraint_type[2] == 1)
    {
        if (profile.Nightshift == constraint.Nightshift)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[2] == 2)
    {
        if (profile.Nightshift == constraint.Nightshift)
            score += 2; // Mandatory weightage
    }
    int totalConstraintScore = constraint.constraint_type[0] + constraint.constraint_type[1] + constraint.constraint_type[2];
    return (score * 100 / totalConstraintScore);
}

float calculateMatchPercentageforJob(seekerprofile profile, recruiterconstraint constraint)
{
    int score = 0;

    // Check constraint_type[0] for mincgpa
    if (constraint.constraint_type[0] == 1)
    {
        if (profile.cgpa >= constraint.mincgpa)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[0] == 2)
    {
        if (profile.cgpa >= constraint.mincgpa)
            score += 2; // Mandatory weightage
    }

    // Check constraint_type[1] for work experience
    if (constraint.constraint_type[1] == 1)
    {
        if (profile.workex >= constraint.minworkex)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[1] == 2)
    {
        if (profile.workex >= constraint.minworkex)
            score += 2; // Mandatory weightage
    }

    // Check constraint_type[2] for qualifications
    if (constraint.constraint_type[2] == 1)
    {
        if (strcmp(profile.qual, constraint.quali) == 0)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[2] == 2)
    {
        if (strcmp(profile.qual, constraint.quali) == 0)
            score += 2; // Mandatory weightage
    }

    // Check constraint_type[3] for location
    if (constraint.constraint_type[3] == 1)
    {
        if (strcmp(profile.loc, constraint.location) == 0)
            score += 1; // Desirable weightage
    }
    else if (constraint.constraint_type[3] == 2)
    {
        if (strcmp(profile.loc, constraint.location) == 0)
            score += 2; // Mandatory weightage
    }

    int totalConstraintScore = constraint.constraint_type[0] + constraint.constraint_type[1] + constraint.constraint_type[2] + constraint.constraint_type[3];
    return (score * 100 / totalConstraintScore);
}

// Function to display top 3 matched job profiles to seeker
void displayTopMatchesforSeeker(jobprofile profiles[], float scores[], int count)
{
    printf("Top 3 Job Matches: \n\n");
    for (int i = 0; i < count && i < 3; i++)
    {
        printf("Job Opening No. %d details: \n", i + 1);
        printf("--------------------------------\n");
        printf("Company name: %s\n", profiles[i].cmpn);
        printf("Job title: %s\n", profiles[i].job);
        printf("Location: %s\n", profiles[i].loc);
        printf("Offered salary: %ld\n", profiles[i].sal);
        printf("Qualifications required: %s\n", profiles[i].qual);
        printf("Skills required: %s\n", profiles[i].skills);
        printf("Minimum work experience required: %f years\n", profiles[i].workex);
        printf("Are night shifts offered?: ");
        if (profiles[i].Nightshift == 1)
        {
            printf("Yes\n\n");
        }
        else if (profiles[i].Nightshift == 0)
        {
            printf("No\n\n");
        }
        printf("Recommendation percentage: %.1f %%\n", scores[i]);
        printf("--------------------------------\n\n");
    }
}

// Function to display top 3 matched seeker profiles 
void displayTopMatchesforRecruiter(seekerprofile profiles[], float scores[], int count)
{
    printf("Top 3 Job Seeker Matches: \n\n");
    for (int i = 0; i < count && i < 3; i++)
    {
        printf("Job Seeker Profile No. %d details: \n", i + 1);
        printf("--------------------------------\n");
        printf("Username: %s \n", profiles[i].seeker_un);
        printf("Gender : %s \n", profiles[i].gen);
        printf("Location : %s \n", profiles[i].loc);
        printf("SSLC Score : %d \n", profiles[i].sslc);
        printf("HSC Score : %d \n", profiles[i].hsc);
        printf("Qualification : %s \n", profiles[i].qual);
        printf("UG CGPA : %f \n", profiles[i].cgpa);
        printf("Work Experience : %f \n", profiles[i].workex);
        printf("Previous Work Designation : %s \n", profiles[i].desig);
        printf("Certifications : %s \n\n", profiles[i].certi);
        printf("Recommendation percentage: %.1f %%\n", scores[i]);
        printf("--------------------------------\n\n");
    }
}

bool IsSeekerProfileExist(char un[])
{
    seekerprofile seekerprofiles[100];
    // Read job profiles from file
    return readSeekerProfiles(seekerprofiles, "seekerprofile.txt", un);
}

void ShowJobSeekerFunctions(char un[])
{
    while (true)
    {
        // after login as job seeker 
        int jobseekermenu = 0;
        printf("-------------------------------\n");
        printf("JOB SEEKER MENU\n");
        printf("-------------------------------\n");
        printf("1: Create Seeker Profile \n");
        printf("2: View Seeker Profile \n");
        printf("3: Set Seeker Constraints \n");
        printf("4: View Matched Jobs \n");
        printf("0: Logout \n\n");
        printf("Select your choice: ");
        scanf("%d", &jobseekermenu);
        printf("-------------------------------\n");

        seekerprofile sp = { "", 0, "", "", 0, 0, "", 0.0, 0.0, "", "" };
        switch (jobseekermenu)
        {
        case 1:
        {
            if (IsSeekerProfileExist(un) > 0)
            {
                printf("Your profile already exists!\n");
                break;
            }
            // create seeker profile
            strcpy(sp.seeker_un, un);
            printf("Enter your age: ");
            scanf("%d", &sp.age);
            printf("Enter your gender (M or F): ");
            scanf("%s", sp.gen);
            printf("Enter your location: ");
            scanf("%s", sp.loc);
            printf("Enter your aggregate SSLC score (out of 500): ");
            scanf("%d", &sp.sslc);
            printf("Enter your aggregate HSC score (out of 500): ");
            scanf("%d", &sp.hsc);
            printf("Enter your qualification (BE/ BTECH/ ME/ MTECH): ");
            scanf("%s", sp.qual);
            printf("Enter your cgpa in UG degree (out of 10): ");
            scanf("%f", &sp.cgpa);
            printf("Enter work experience (If you have no work experience, Enter 0): ");
            scanf("%f", &sp.workex);
            printf("Enter your designation in the previously worked institution (If you have no work experience, Enter NIL): ");
            scanf("%s", sp.desig);
            printf("Enter your certification in the previously worked institution (If you have no certifications, Enter NIL): ");
            scanf("%s", sp.certi);

            FILE* fsp = fopen("seekerprofile.txt", "a");
            if (fsp != NULL)
            {
                fprintf(fsp, "%s %d %s %s %d %d %s %f %f %s %s \n", sp.seeker_un, sp.age, sp.gen, sp.loc, sp.sslc, sp.hsc, sp.qual, sp.cgpa, sp.workex, sp.desig, sp.certi);
                fclose(fsp);
                printf("Job Profile succesfully created! \n\n");
            }
            else
            {
                printf("Error opening file. Please try again. \n\n");
            }
            break;
        }

        case 2:
        {
            seekerprofile sprofiles[100];
            if (readSeekerProfiles(sprofiles, "seekerprofile.txt", un) > 0)
            {

                for (int x = 0; x < (sizeof(sprofiles) / sizeof(sprofiles[0])); x++)
                {
                    if (strcmp(sprofiles[x].seeker_un, un) == 0)
                    {
                        printf("Username : %s \n", sprofiles[x].seeker_un);
                        printf("Age : %d \n", sprofiles[x].age);
                        printf("Gender : %s \n", sprofiles[x].gen);
                        printf("Location : %s \n", sprofiles[x].loc);
                        printf("SSLC Score : %d \n", sprofiles[x].sslc);
                        printf("HSC Score : %d \n", sprofiles[x].hsc);
                        printf("Qualification : %s \n", sprofiles[x].qual);
                        printf("UG CGPA : %f \n", sprofiles[x].cgpa);
                        printf("Work Experience : %f \n", sprofiles[x].workex);
                        printf("Previous Work Designation : %s \n", sprofiles[x].desig);
                        printf("Certifications : %s \n", sprofiles[x].certi);
                        break;
                    }
                }
            }
            break;
        }

        case 3:
        {
            // printf and scanf to ask for constrints and write into seekerconstraints.txt

            seekerconstraint sc1 = { "", 0, "", false, 1, 1, 1 };

            strcpy(sc1.seeker_un, un);
            printf("Please enter job seeker constraints: \n");
            printf("Enter constraint 1: Minimum Salary (INR): ");
            scanf("%ld", &sc1.minsalary);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &sc1.constraint_type[0]);
            printf("Enter constraint 2: Location (Enter a string): ");
            scanf("%s", sc1.location);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &sc1.constraint_type[1]);
            printf("Enter constraint 3: Are you ok with night shifts? (Enter 1 for Yes, 0 for No): ");
            scanf("%d", &sc1.Nightshift);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &sc1.constraint_type[2]);

            FILE* fp;
            fp = fopen("seekerconstraint.txt", "a");
            if (fp == NULL)
            {
                printf("Error opening file. Please try again. \n");
            }
            else
            {
                fprintf(fp, "%s %ld %s %d %d %d %d\n", sc1.seeker_un, sc1.minsalary, sc1.location, sc1.Nightshift, sc1.constraint_type[0], sc1.constraint_type[1], sc1.constraint_type[2]);
                printf("Job seeker constraints applied successfully!!!\n\n");
            }
            fclose(fp);
            break;
        }
        case 4:
        {
            jobprofile profiles[100];
            seekerconstraint constraints[100];
            int profileCount, constraintCount;

            // Read job profiles from file
            profileCount = readJobProfiles(profiles, "jobprofile.txt");
            if (profileCount == 0)
            {
                printf("No job profiles found!\n");
                break;
            }

            // Read seeker constraints from file
            char seekerUsername[50];
            strcpy(seekerUsername, un);
            constraintCount = readSeekerConstraints(constraints, "seekerconstraint.txt", seekerUsername);
            if (constraintCount == 0)
            {
                printf("No seeker constraints found for username: %s\n", seekerUsername);
                break;
            }

            // Calculate confidence scores against each job profile
            float scores[100];
            for (int i = 0; i < profileCount; i++)
            {
                scores[i] = calculateMatchPercentageforSeeker(profiles[i], constraints[constraintCount - 1]);
            }

            // Sort scores and corresponding profiles in descending order
            for (int i = 0; i < profileCount - 1; i++)
            {
                for (int j = i + 1; j < profileCount; j++)
                {
                    if (scores[j] > scores[i])
                    {
                        // sorting scores 
                        float tempScore = scores[i];
                        scores[i] = scores[j];
                        scores[j] = tempScore;

                        // Sorting structures inside the array of structures jobprofile along with its corresponding score
                        jobprofile tempProfile = profiles[i];
                        profiles[i] = profiles[j];
                        profiles[j] = tempProfile;
                    }
                }
            }

            // Display top matched job profiles
            displayTopMatchesforSeeker(profiles, scores, profileCount);
            break;
        }
        case 0:
        {
            return;
        }
        default:
        {
            printf("Invalid, please try again !\n");
        }
        }// end switch
    }// end while
}


void ShowRecruiterFunctions(char un[])
{
    while (true)
    {
        int recruitermenu = 0;
        printf("-------------------------------\n");
        printf("TALENT ACQUISITION MANAGER MENU \n");
        printf("-------------------------------\n");
        printf("1: Create Job Profiles \n");
        printf("2: View Job Profiles \n");
        printf("3: Set Recruiter Constraints \n");
        printf("4: View Matched Job Seekers \n");
        printf("0: Logout \n\n");
        printf("Select your choice: ");
        scanf("%d", &recruitermenu);
        printf("-------------------------------\n");

        jobprofile jp = { "", "", "", "", "", "", 0, 0.0, 0 };

        switch (recruitermenu)
        {
        case 1:
        {
            // create job opening/ profile
            strcpy(jp.recruiter_un, un);
            printf("Enter company name: ");
            scanf("%s", jp.cmpn);
            printf("Enter job title: ");
            scanf("%s", jp.job);
            printf("Enter qualifications required (BE/ BTECH/ ME/ MTECH): ");
            scanf("%s", jp.qual);
            printf("Enter skills required: ");
            scanf("%s", jp.skills);
            printf("Enter location: ");
            scanf("%s", jp.loc);
            printf("Enter offered salary: ");
            scanf("%ld", &jp.sal);
            printf("Enter minimum work experience required (in years): ");
            scanf("%f", &jp.workex);
            printf("Do you offer night shifts? (Enter 1 for Yes, 0 for No): ");
            scanf("%d", &jp.Nightshift);

            FILE* fjp = fopen("jobprofile.txt", "a");
            if (fjp != NULL)
            {
                fprintf(fjp, "%s %s %s %s %s %s %ld %f %d \n", jp.recruiter_un, jp.cmpn, jp.job, jp.qual, jp.skills, jp.loc, jp.sal, jp.workex, jp.Nightshift);
                fclose(fjp);
                printf("Job Profile succesfully created! \n");
            }
            else
            {
                printf("Error opening file. Please try again. \n");
            }
            break;
        }
        case 2:
        {
            int profileCount, jobcount = 0;
            jobprofile profiles[100];
            profileCount = readJobProfiles(profiles, "jobprofile.txt");
            for (int x = 0; x < profileCount; x++)
            {
                if (strcmp(profiles[x].recruiter_un, un) == 0)
                {
                    printf("-------------------------\n");
                    printf("Job No. %d: \n", jobcount + 1);
                    printf("-------------------------\n");
                    printf("Company name: %s\n", profiles[x].cmpn);
                    printf("Job title: %s\n", profiles[x].job);
                    printf("Location: %s\n", profiles[x].loc);
                    printf("Offered salary: %ld\n", profiles[x].sal);
                    printf("Qualifications required: %s\n", profiles[x].qual);
                    printf("Skills required: %s\n", profiles[x].skills);
                    printf("Minimum work experience required: %f years\n", profiles[x].workex);
                    printf("Are night shifts offered?: ");
                    if (profiles[x].Nightshift == 1)
                    {
                        printf("Yes\n");
                    }
                    else if (profiles[x].Nightshift == 0)
                    {
                        printf("No\n");
                    }

                    jobcount++;
                }
            }
            printf("-------------------------\n");
            printf("Number of jobs found: %d\n", jobcount);
            printf("-------------------------\n");
            break;
        }
        case 3:
        {
            recruiterconstraint rc1 = { "", "", 0.0, 0.0, "", "", 1, 1, 1, 1 };

            strcpy(rc1.recruiter_un, un);
            printf("Please enter Job title for the job that you want to add a constraint for: ");
            scanf("%s", rc1.job);
            printf("Please enter recruiter constraints: \n");
            printf("Enter constraint 1: Minimum CGPA (out of 10): ");
            scanf("%f", &rc1.mincgpa);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &rc1.constraint_type[0]);
            printf("Enter constraint 2: Minimum work experience (in years): ");
            scanf("%f", &rc1.minworkex);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &rc1.constraint_type[1]);
            printf("Enter constraint 3: Required qualifications (BE/ BTECH/ ME/ MTECH): ");
            scanf("%s", rc1.quali);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &rc1.constraint_type[2]);
            printf("Enter constraint 4: Location (Enter a string): ");
            scanf("%s", rc1.location);
            printf("Enter whether this constraint is Desirable(1) or Mandatory(2): ");
            scanf("%d", &rc1.constraint_type[3]);

            FILE* fp;
            fp = fopen("recruiterconstraint.txt", "a");
            if (fp == NULL)
            {
                printf("Error opening file. Please try again. \n");
            }
            else
            {
                fprintf(fp, "%s %s %f %f %s %s %d %d %d %d\n", rc1.recruiter_un, rc1.job, rc1.mincgpa, rc1.minworkex, rc1.quali, rc1.location, rc1.constraint_type[0], rc1.constraint_type[1], rc1.constraint_type[2], rc1.constraint_type[3]);
                printf("Recruiter constraints applied successfully!!!\n\n");
            }
            fclose(fp);
            break;
        }
        case 4:
        {
            seekerprofile profiles[100];
            recruiterconstraint constraints[100];
            int profileCount, constraintCount;
            char JobTitle[50];

            printf("Please enter Job title for the job that you want to see candidate matches for: ");
            scanf("%s", JobTitle);

            // Read seeker profiles from file
            profileCount = readSeekerProfiles(profiles, "seekerprofile.txt", "");
            if (profileCount == 0)
            {
                printf("No seeker profiles found!\n");
                break;
            }

            // Read recruiter constraints from file
            char recruiterUsername[50];
            strcpy(recruiterUsername, un);
            constraintCount = readRecruiterConstraints(constraints, "recruiterconstraint.txt", recruiterUsername, JobTitle);
            if (constraintCount == 0)
            {
                printf("No recruiter constraints found for username: %s\n", recruiterUsername);
                break;
            }

            // Calculate confidence scores against each seeker profile
            float scores[100];
            for (int i = 0; i < profileCount; i++)
            {
                scores[i] = calculateMatchPercentageforJob(profiles[i], constraints[constraintCount - 1]);
            }

            // Sort scores and corresponding profiles in descending order
            for (int i = 0; i < profileCount - 1; i++)
            {
                for (int j = i + 1; j < profileCount; j++)
                {
                    if (scores[j] > scores[i])
                    {
                        // sorting scores 
                        float tempScore = scores[i];
                        scores[i] = scores[j];
                        scores[j] = tempScore;

                        // Sorting structures inside the array of structures jobprofile along with its corresponding score
                        seekerprofile tempProfile = profiles[i];
                        profiles[i] = profiles[j];
                        profiles[j] = tempProfile;
                    }
                }
            }

            // Display top matched job profiles
            displayTopMatchesforRecruiter(profiles, scores, profileCount);
            break;
        }
        case 0:
        {
            return;
        }
        default:
        {
            printf("Invalid, please try again !\n");
        }
        }// end switch
    }// end while
}

void main()
{
    int opt, l;
    printf("Welcome to the Job Portal! \n\n");
    while (true)
    {
        printf("------------------------------- \n");
        printf("MAIN MENU \n");
        printf("------------------------------- \n");
        printf("1. Seeker Registration \n");
        printf("2. Recruiter Registration \n");
        printf("3. Login \n");
        printf("0. Quit \n");
        printf("Enter your choice: ");
        scanf("%d", &opt);
        printf("------------------------------- \n");
        printf("\n");
        switch (opt)
        {
        case 1:
        {
            register_seeker();
            break;
        }
        case 2:
        {
            register_recruiter();
            break;
        }
        case 3:
        {
            char un[50];
            l = login(un);
            if ((l == 1) && (l != 0))
            {
                ShowJobSeekerFunctions(un);
            }
            else if ((l == 2) && (l != 0))
            {
                ShowRecruiterFunctions(un);
            }
            break;
        }
        case 0:
        {
            printf("Thank you for visiting our Job Portal! Have a nice day :) \n");
            exit(0);
            break;
        }
        default:
        {
            printf("Invalid choice. Please try again.\n");
        }
        }// end switch
        printf("\n");
    }//end while
}

