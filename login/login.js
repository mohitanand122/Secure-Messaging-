document.addEventListener("DOMContentLoaded", function () {
    const usernameInput = document.getElementById("username");
    const passwordInput = document.getElementById("password");
    const signInButton = document.querySelector("button[type='submit']");

    
    const usernamePattern = /^[a-zA-Z0-9_@.]+$/;

    function validateForm() {
        const username = usernameInput.value.trim();
        const password = passwordInput.value.trim();

        
        const isUsernameValid = usernamePattern.test(username);

        if (username !== "" && password !== "" && isUsernameValid) {
            signInButton.disabled = false;
            signInButton.classList.remove("btn-disabled");
            signInButton.classList.add("btn-enabled");
        } else {
            signInButton.disabled = true;
            signInButton.classList.remove("btn-enabled");
            signInButton.classList.add("btn-disabled");
        }

        if (!isUsernameValid) {
            usernameInput.setCustomValidity("Username can only contain letters, numbers, underscores, and @.");
        } else {
            usernameInput.setCustomValidity(""); 
        }
    }

   
    usernameInput.addEventListener("input", validateForm);
    passwordInput.addEventListener("input", validateForm);

   
    // document.querySelector("form").addEventListener("submit", function (e) {
    //     e.preventDefault();
    //     if (!signInButton.disabled) {
    //         alert("Form submitted!");
    //     }
    // });
});
