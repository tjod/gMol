let geom;
let winy = 400;
let winx = 400;
let tscale = 100;
async function setup() {
  let font = await loadFont("../assets/EduQLDHand-Regular.ttf");
  createCanvas(winx, winy, WEBGL);
  geom = font.textToModel("This is Gramps", 10, 10, { sampleFactor: 2, extrude: 4 });
  geom.clearColors();
  geom.normalize();
  noLoop();
}

function draw() {
  background(255);
  orbitControl();
  fill("blue");
  strokeWeight(4);
  scale(min(width, height) / 300);
  model(geom);

  let myWriter = createWriter("block.pix");
  let geometryData = {
    vertices: geom.vertices.map(v => [v.x, v.y, v.z]), // Array of [x,y,z]
    faces: geom.faces,                                 // Vertex index mapping
    uvs: geom.uvs,                                     // Texture coordinates
    vertexNormals: geom.vertexNormals.map(n => [n.x, n.y, n.z])
  };
  //myWriter.write([geometryData.vertices.length,7,3]); //  mesh no normals
  //myWriter.write([geometryData.vertices.length,7,1]); // mesh with normals
  myWriter.write([geometryData.vertices.length,7,3]);
  myWriter.write("\n");
  for (const v of geometryData.vertices) {
    // -y corrects coord system based on upper left, but reverses clockwise-ness
    myWriter.write([ (v[0]/tscale).toFixed(3), (-v[1]/tscale).toFixed(3), (v[2]/tscale).toFixed(3)]);
    // normals don't add anything, since they're based on flat traingles and don't account
    // for any curviness of the letters
    myWriter.write("\n");
  }
  // use clockwise triangles because of inversion to -y above
  myWriter.write([geometryData.faces.length,8,-1]); // cw mesh onnectivity
  myWriter.write("\n")
  for (const f of geometryData.faces) {
    myWriter.write([f[0]+1, f[1]+1, f[2]+1]); // vertices start with 1, not 0
    myWriter.write("\n");
  }
  myWriter.close();

  describe('3D extruded string.');
}
