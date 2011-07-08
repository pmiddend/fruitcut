hideables = document.getElementsByClassName("hideable");
showhides = document.getElementsByClassName("showhide");

function toggle(div, button) {
	arrow = button.getElementsByClassName("arrow")[0]
	if(div.style.display == "block") {
		arrow.setAttribute("src", "arrow.png");
		div.style.display = "none";
	}
	else {
		arrow.setAttribute("src", "arrow_r.png");
		div.style.display = "block";
	}
} 

window.addEventListener("load", function (event) {
	init();
}, false);

function init() {
	var i = 0;
	var elem;

	for (i = 0; i < showhides.length; ++i)
	{
		elem = showhides[i];
		var arrow = document.createElement("img");
		arrow.setAttribute("class", "arrow");
		arrow.setAttribute("src", "arrow.png");
		elem.appendChild(arrow);
	}
	for (i = 0; i < hideables.length; ++i)
	{
		elem = hideables[i];
		elem.style.display = "none";
	}
	for (i = 0; i < showhides.length; ++i)
	{
		elem = showhides[i];
		elem.addEventListener("click", function (e) {
			hideable_div = this.nextElementSibling;
			toggle(hideable_div, this);
			},
			false);
		elem.addEventListener("mouseover", function (e) {
			this.style.textDecoration = "underline";
			}, false);
		elem.addEventListener("mouseout", function (e) {
			this.style.textDecoration = "none";
			}, false);
	}
}
