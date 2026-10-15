document.addEventListener('click', function(event) {
  const link = event.target.closest('a[href]');
  if (link) {
    //console.log(link);
    // 1. Stop the browser from navigating automatically
    event.preventDefault();
    // 2. Extract link information
    const relativeHref = link.getAttribute('href');
    const absoluteUrl = link.href;
    
    console.log(`Intercepted click to: ${relativeHref}`)
    // undisplay all iframes
    Array.from(document.querySelectorAll('iframe')).forEach(iframe => {
      iframe.style.display = 'none';
    });
    // get the iframe (name same as link) and display that one
    var iframe = document.getElementsByName(relativeHref)[0];
    iframe.style.display = "block"; // restore

    window.scrollTo({
      top: 0,
      behavior: 'smooth'
    });
  }
});

document.addEventListener("DOMContentLoaded", () => {
    var iframe = document.getElementsByName('introduction.html')[0];
    iframe.style.display = "block"; // restore
});
