const button = document.getElementById("button");
const text = document.getElementById("text");
// const form = document.getElementById("form");
let i = 0;
function increament(event){
    i++;
    text.textContent = `${i}`;
}
// async function sendData(event){
//     event.preventDefault();
//     let res = await fetch("/admin",{
//         method: "POST",
//         headers:{
//             "Content-Type": "application/json"
//         },
//         body: JSON.stringify({
//             "name": document.getElementById("name").value,
//             "email": document.getElementById("email").value,
//             "password": document.getElementById("password").value
//         })
//     });
// }
button.addEventListener("click",increament);
// form.addEventListener("submit",sendData);