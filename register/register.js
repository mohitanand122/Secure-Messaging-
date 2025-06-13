// Function to toggle password visibility
function togglePasswordVisibility(inputId, icon) {
    const input = document.getElementById(inputId);
    if (input.type === "password") {
        input.type = "text";
        icon.classList.remove("fa-eye");
        icon.classList.add("fa-eye-slash");
    } else {
        input.type = "password";
        icon.classList.remove("fa-eye-slash");
        icon.classList.add("fa-eye");
    }
}

// Validate passwords on input
document.addEventListener("DOMContentLoaded", function () {
    const passwordInput = document.getElementById("password");
    const repeatPasswordInput = document.getElementById("repeat-password");
    const registrationForm = document.getElementById("registrationForm");
    const errorMessage = document.getElementById("error-message");

    function validatePasswords() {
        if (passwordInput.value !== repeatPasswordInput.value) {
            repeatPasswordInput.setCustomValidity("Passwords do not match.");
            errorMessage.textContent = "Passwords do not match."; // Display error message
            errorMessage.style.display = "block"; // Show error message
        } else {
            repeatPasswordInput.setCustomValidity(""); // Reset if they match
            errorMessage.textContent = ""; // Clear error message
            errorMessage.style.display = "none"; // Hide error message
        }
    }

    // Debounce function to limit validation calls
    function debounce(func, delay) {
        let timeout;
        return function (...args) {
            clearTimeout(timeout);
            timeout = setTimeout(() => func.apply(this, args), delay);
        };
    }

    const debouncedValidatePasswords = debounce(validatePasswords, 300);

    passwordInput.addEventListener("input", debouncedValidatePasswords);
    repeatPasswordInput.addEventListener("input", debouncedValidatePasswords);

    registrationForm.addEventListener("submit", function (e) {
        validatePasswords(); // Ensure validation before submission
        if (!repeatPasswordInput.checkValidity()) {
            e.preventDefault(); // Prevent submission if passwords don't match
            errorMessage.textContent = "Passwords do not match!"; // Show error message
            errorMessage.style.display = "block"; // Show error message
        }
    });
});