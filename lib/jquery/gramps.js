function docmd(e) {
	var cmd = $("#cmd_line").val();
	var resp = gramps.cmd(cmd);
	console.log(resp.length);
	if (resp.length > 0) $("#cmd_out").prepend(resp+"\n");
	$("#cmd_out").prepend("<p class=cmd_echo> "+cmd+"</p>");
	$("#cmd_line").val("");
	return false;
}
$(document).ready( function() {
	$("#tabs").tabs();
	$("#tabs").on("tabsactivate", function( event, ui ) {
		var active = $("#tabs .ui-state-active a").attr('href');
		if (active == "#command") {
			$("#cmd_line").focus();
		} else if (active == "#tree") {
			$("#tree").html(gramps.cmd("show/tree"));
		} else if (active == "#system") {
			$("#system").html(gramps.cmd("show/system"));
		}
	} );
	$("#cmd_line").focus();
	$("form").submit(docmd);
});
