async function loadComponent(id, file) {
    const res = await fetch(file);
    const html = await res.text();
    document.getElementById(id).innerHTML = html;
}

document.addEventListener("DOMContentLoaded", async () => {
    loadComponent("header-container", "/components/header.html");
    loadComponent("footer-container", "/components/footer.html");

    document.addEventListener("click", (e) => {
        if (e.target.id === "nav-home") window.location.href = "/index.html";
        if (e.target.id === "nav-setting") window.location.href = "/settings.html";
    });
});
