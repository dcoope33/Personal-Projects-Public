const loginForm = document.querySelector('.login-form');
const signupForm = document.querySelector('.signup-form');
const showSignup = document.querySelector('#show-signup');
const showLogin = document.querySelector('#show-login');
const work = document.querySelector('.identify');

document.addEventListener('DOMContentLoaded', () => {

    const signinbtn = document.querySelector('.menucontainer a:nth-child(3)');
    const myplansbtn = document.querySelector('.menucontainer a:nth-child(4)');
    const signoutbtn = document.querySelector('.menucontainer a:nth-child(5)');
    const options = document.getElementById('options');
    const add1 = document.getElementById('add1');
    const add2 = document.getElementById('add2');
    const no_plan1 = document.getElementById('no_plan1');
    const no_plan2 = document.getElementById('no_plan2');
    const plan1 = document.getElementById('plan1');
    const plan2 = document.getElementById('plan2');
    const upgrade = document.getElementById('upgrade');
    const free = document.getElementById('free');
    const tier1 = document.getElementById('tier1');
    const tier2 = document.getElementById('tier2');
    const view = document.getElementById('view');

    // 1. get user info from server
    fetch('/check-auth')
        .then(response => response.json())
        .then(data => {
            if (data.loggedIn) {

                // User is logged in: Hide Sign In, Show My Plans
                if (signinbtn) signinbtn.hidden = true;
                if (myplansbtn) myplansbtn.hidden = false;
                if (signoutbtn) signoutbtn.hidden = false;
                

                if (work) {
                    const worktype = document.querySelector('.identify').textContent;
                    console.log(worktype);

                    // code for the dynamic plan buttons
                    if (options && data.tier == 'none') {
                        options.hidden = false;
                    } else if (options && data.plan1 == 'none' && data.plan2 != worktype) {
                        add1.hidden = false;
                    } else if (options && data.tier == 'T2' && data.plan2 == 'none' && data.plan1 != worktype) {
                        add2.hidden = false;
                    } else if (options && data.plan1 == worktype || data.plan2 == worktype){
                        view.hidden = false;
                    }
                }

                // code for the dynamic plan1 divs
                if (plan1 && data.tier != 'none' && data.plan1 == 'none') {
                    no_plan1.hidden = false;
                } else if (plan1) {
                    plan1.hidden = false;
                    document.getElementById('p1text').innerText = `You Have the ${data.plan1} Plan`;
                }

                // code for the dynamic plan2 divs
                if (plan2 && data.tier != 'T2') {
                    upgrade.hidden = false;
                } else if (plan2 && data.plan2 == 'none') {
                    no_plan2.hidden = false;
                } else if (plan2) {
                    plan2.hidden = false;
                    document.getElementById('p2text').innerText =  `You Have the ${data.plan2} Plan`;
                }

                // code for the dynamic tier marks
                if (free && data.tier == 'free') {
                    free.hidden = false;
                } else if (tier1 && data.tier == 'T1') {
                    tier1.hidden = false;
                } else if (tier2 && data.tier == 'T2') {
                    tier2.hidden = false;
                }

            } else {
                if (signinbtn) signinbtn.hidden = false;
                if (options) options.hidden = false; 
            }
            })
            .catch(err => console.error("Auth check failed:", err));
    });

    // signin/signup switching
    if(showSignup) {
        console.log("Signup button found!");
        showSignup.addEventListener('click', (e) => {
            e.preventDefault();
            console.log("Signup clicked!");
            loginForm.classList.add('hidden');
            signupForm.classList.remove('hidden');
        });
    }

    if(showLogin) {
        showLogin.addEventListener('click', (e) => {
            e.preventDefault();
            signupForm.classList.add('hidden');
            loginForm.classList.remove('hidden');
        });
}
