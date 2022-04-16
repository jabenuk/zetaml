const HighlightColour = "#d6b8f1";

function highlight(c) {
	// clear old highlights
	// !! this removes all background colours, so only do this on actual-index.html !!
	document.querySelectorAll("*").forEach(e => {
		e.style.backgroundColor = "";
	});

	let elems = document.querySelectorAll(c);
	elems.forEach(e => {
		e.style.backgroundColor = HighlightColour;
	});
}
function clearhl(c) {
	// !! this removes all background colours, so only do this on actual-index.html !!
	document.querySelectorAll("*").forEach(e => {
		e.style.backgroundColor = "";
	});
}