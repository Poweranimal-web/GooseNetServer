const button = document.getElementById("button");
const text = document.getElementById("text");
let i = 0;
function increament(event){
    i++;
    text.textContent = `${i}`;
}
button.addEventListener("click",increament);