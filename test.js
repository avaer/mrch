const mrch = require('.');

const NUM_CELLS = 16;
const NUM_CELLS_OVERSCAN = NUM_CELLS + 1;

const _getEtherIndex = (x, y, z) => x + (z * NUM_CELLS_OVERSCAN) + (y * NUM_CELLS_OVERSCAN * NUM_CELLS_OVERSCAN);

const ether = new Float32Array(NUM_CELLS_OVERSCAN * NUM_CELLS_OVERSCAN * NUM_CELLS_OVERSCAN);
ether.fill(1);
ether[_getEtherIndex(4, 4, 4)] = -1;

const positionsBuffer = new Float32Array(new ArrayBuffer(500 * 1024));
const facesBuffer = new Uint32Array(new ArrayBuffer(500 * 1024));

const result = mrch.marchingCubes(
  [NUM_CELLS + 1, NUM_CELLS + 1, NUM_CELLS + 1],
  ether,
  [[0, 0, 0], [0, 100, 0]],
  positionsBuffer,
  facesBuffer
);

console.log(result);

/* let min = Infinity;
let max = -Infinity;
console.time('time');
for (let dx = 0; dx < 64; dx++) {
  for (let dz = 0; dz < 64; dz++) {
    const v = fn.in2D(1000 + dx, 1000 + dz);
    min = Math.min(v, min);
    max = Math.max(v, max);
  }
}
console.timeEnd('time');
console.log(min, max); */
