DISCLAIMER:

    As this is a concept website the dynamic workout plans and quizzes are not in affect.


OVERVIEW:

        This is a concept website that I have built that centers around semi custom workout plans
    It has a somewhat minimilistic interphase and there are 6 different plan types. Each plan type
    has its own page that in theory would have a very in depth description of what the plan would 
    entail (currentely each plan page just lets you add it as one of your plans if you are signed in).
    There are three different user tiers free, teir1, and tier2. Each tier has different levels of access
    to the websites resources. When you create an account and select a tier you are allowed to add up to 
    two plans (depending on what tier you have). when you have a plan added there is in theory a quiz you 
    could take in order to benchmark your progress (doesn't exist but it's a concept). 
    

        How the concept of the semi custom workouts would work is that for whatever plan you have you take the quiz 
    and that will be your baseline. Every day the plans page would display your current workout for the day (pulled from 
    the SQL database). After your do your workout for the day you can take the quiz again to update your progress. The 
    workouts would increase in difficulty based on your progress. This concept is great for semi custom plans since 
    everything would be automated. 


        In its current state when you add a plan and access it the page just shows a single hardcoded workout and a quiz 
    that you cannot actually take. This is because this is a concept website and actually making a bunch of workout
    plans is difficult and takes time. In addition to this coming up with a quiz that would make sense to go along with
    the workout plans would also take time. 



DESIGN:

        For this website I am using nodejs for the server backend along with sqlite3 for the database. I also make use of 
    bcrypt for the password encryption. For the design I am not doing anything too crazy as most of my server is just waiting 
    for GETs or POSTs and handling them accordingly. Some of the hadlers communicate with the SQL database as well. I added 
    password encryption to make it "more secure" (It's just a concept site so doesn't really matter). 


GOALS:

        The main goal with this project was to farmiliarize myself with basic web development. 
    I wanted to get more farmiliar with basic HTML, CSS, and Javascript while also practice my SQL skills. 
    As an added bonus I am re-writing the backend in C# as a way to practice that language as well.


ADDITIONAL NOTES:

    1.  As this website uses nodejs for the backend and to localhost it requires it to be installed to work.
        (sudo apt update && sudo apt install nodejs npm -y) for Ubuntu .

    2.  AS this website uses sqlite3 for the database it also requires that to be installed.
        (sudo apt update && sudo apt install sqlite3) for Ubuntu.

    3.  As this website uses bcrypt it is a requirement that that is installed as well.
        npm install bcrypt.
