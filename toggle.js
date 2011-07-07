hideables = document.getElementsByClassName("hideable");
showhide = document.getElementsByClassName("showhide");

function toggle(div, button) {
	if(div.style.display == "inline") {
		div.style.display = "none";
		button.innerHTML = "show";
	}
	else {
		div.style.display = "inline";
		button.innerHTML = "hide";
	}
} 

window.addEventListener("load", function (event) {
	init();
}, false);

function init() {
	var i = 0;
	var elem;
	for (i = 0; i < hideables.length; ++i)
	{
		elem = hideables[i];
		elem.style.display = "none";
	}
	for (i = 0; i < showhide.length; ++i)
	{
		elem = showhide[i];
		elem.style.display = "inline";
		elem.addEventListener("click", function (e) {
			instr = this.parentNode.nextElementSibling;
			toggle(instr, this);
			},
			false);
		/* hacky... */
		elem.addEventListener("mouseover", function (e) {
			this.style.color = "#fec";
			this.style.backgroundColor = "#372700";
			}, false);
		elem.addEventListener("mouseout", function (e) {
			this.style.color = "#372700";
			this.style.backgroundColor = "#fec";
			}, false);
	}
}
