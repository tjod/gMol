function docmd(e) {
	var command = $("#cmd_line").val();
	var response = gramps.cmd(command);
	// show response
	if (response.length > 0) $("#cmd_out").prepend(response+"\n");
	// echo command
	$("#cmd_out").prepend("<p class=cmd_echo> "+command+"</p>");
	// remove command
	$("#cmd_line").val("");
	// don't actually submit the form
	return false;
}
function showobj(e) {
	var name = this.innerHTML;
	// just the non-default values
	var html = gramps.cmd("save/dial " + name);
	html += "<h2>" + name + "</h2>";
	// all values
	html += gramps.cmd("show/all " + name);
	$("#showobj").html(html);
}
function showtree() {
	gramps.cmd("set noecho");
	var tree = JSON.parse(gramps.cmd("json/tree"));
	var html = "<a class='tree'>LWorld</a><ul class='tree'>";
	html += shownodes(tree.LWorld, "LWorld");
	html += "</ul>";
	console.log(html);
	$("#showtree").html(html);
	// when a tree item is clicked
	$("a.tree").on("click", showobj);
}
function shownodes(tree, node) {
	var html = "";
	jQuery.each(tree, function(key,val) {
		html += "<li class='tree'><a class='tree'>" + val.name + "</a>";
		if (val.children) {
			html += "<ul class='tree'>" + shownodes(val.children, val.name) + "</ul>";
		}
		html += "</li>";
	});
	return html;
}
// standard jquery setup
$(document).ready( function() {
	// initialize the tabs div
	$("#tabs").tabs();
	// when one or another tab is selected
	$("#tabs").on("tabsactivate", function( event, ui ) {
		// the href/name of the active tab
		var active = $("#tabs .ui-state-active a").attr('href');
		if (active == "#command") {
			$("#cmd_line").focus();
		} else if (active == "#tree") {
			showtree();
		} else if (active == "#system") {
			$("#system").html(gramps.cmd("show/system"));
		}
	} );
	$("#cmd_out").on("click", function(e) {
	        console.log(window.getSelection().toString());
	});
	// let command input have focus initially
	$("#cmd_line").focus();
	// do the command when the user types return (submits the form)
	$("form").submit(docmd);
});
