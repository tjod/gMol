Ext.define('Gramps.view.Demo' ,{
    extend: 'Ext.Panel',
    alias : 'widget.demo',
    title: 'Demos',
    autoScroll: true,
    initComponent: function() {
		//this.html = "Not sure how to make demos/index.html come up here";
		//this.html += "<a href='x.html' target=_blank>Demos</a>";
   		//this.loader = {url: "file://../../demos/index.html", renderer: "html", autoLoad: true};
		this.html = "<a href='../../demos/index.html' target=demoFrame>Demos</a>" +
				"<iframe name=demoFrame width='100%' height='100%'></iframe>"
        this.callParent(arguments);
    }
});