let font;
let tscale = 100;

async function setup() {
  createCanvas(400, 400);
  font = await loadFont("../assets/Roboto-Regular.ttf");
  noLoop();
}

function draw() {
  background(200);
  textAlign(CENTER, CENTER);
  textSize(50);
  let myWriter = createWriter("text.pix");
  // Get the point array.
  let contours = font.textToContours("This is Gramps", width / 2, height / 2, {
    sampleFactor: 0.5,
  });
  var n = 0;
  beginShape();
  for (const pts of contours) {
    beginContour();
    myWriter.write([2*contours[n++].length,2,3,"\n"]);
    for (const pt of pts) {
      vertex(pt.x, pt.y);
      myWriter.write([(pt.x/tscale-2).toFixed(3), (-pt.y/tscale+2).toFixed(3),  0.05]);
      myWriter.write("\n");
      myWriter.write([(pt.x/tscale-2).toFixed(3), (-pt.y/tscale+2).toFixed(3), -0.05]);
      myWriter.write("\n");
    }
    endContour(CLOSE);
  }
  endShape();
  myWriter.close();
}
