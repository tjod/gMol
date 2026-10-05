var cmd_history = [];
var cmd_back = 0;

function docmd(e) {
	var command = $("#cmd_line").val();
	cmd_history.push(command);
        cmd_back = 0;
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
	var html = "";
	html += "<p class='cmd_show'>save/dial " + name + "</p>";
	html += gramps.cmd("save/dial " + name);
	html += "<p class='cmd_show'>show/all " + name + "</p>";
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
        // make sure all is visible by scrolling to the top
        window.scrollTo({
          top: 0,
          behavior: 'smooth'
        });

  //  up-arrow or down-arrow scrolls previously entered commands
  const inputElement = document.getElementById('cmd_line');
  var cmd = undefined;
  inputElement.addEventListener('keydown', function(event) {
    // Check if the pressed key is the Up Arrow
    if (event.which === 38) {
      // Prevent default browser behavior (e.g., cursor jumping to start)
      event.preventDefault();
      cmd_back += 1;
    // Check if the pressed key is the Down Arrow
    } else if (event.which === 40) {
      event.preventDefault();
      cmd_back -= 1;
    } else {
      return;
    }
    // if no command history just return
    if (cmd_history.length == 0) {
      return;
    }
    // keep cmd_back array pointer in range
    if (cmd_back < 1) {
      cmd_back = 1;
    } else if (cmd_back > cmd_history.length) {
      cmd_back = cmd_history.length;
    }
    // show the previous command
    inputElement.value = cmd_history[cmd_history.length - cmd_back];
  });

});
