// Find the latest version by visiting https://unpkg.com/three. The URL will
// redirect to the newest stable release.
import * as THREE from 'https://unpkg.com/three@0.123.0/build/three.module.js';
// Orbit controls
import { OrbitControls } from 'https://unpkg.com/three@0.123.0/examples/jsm/controls/OrbitControls.js';

// ------------------------------------------------
// BASIC SETUP
// ------------------------------------------------

// Create an empty scene
var scene = new THREE.Scene();

var container = document.getElementById('canvas-container');
console.log(container.offsetWidth, container.offsetHeight);
// Create a basic perspective camera
var camera = new THREE.PerspectiveCamera( 75, container.offsetWidth/container.offsetHeight, 0.1, 1000 );
camera.position.z = 4;

// Create a renderer with Antialiasing
var renderer = new THREE.WebGLRenderer({antialias:true});

// Configure renderer clear color
renderer.setClearColor("#f4f4f4");

// Configure renderer size
renderer.setSize( container.offsetWidth, container.offsetHeight );

// Append Renderer to DOM
container.appendChild( renderer.domElement );

// ------------------------------------------------
// FUN STARTS HERE
// ------------------------------------------------

// Add orbit controls to the scene.
const controls = new OrbitControls( camera, renderer.domElement );
controls.minDistance = 10;

// controls.update() must be called after any manual changes to the camera's transform, example.
// camera.position.set( 0, 20, 100 );
// controls.update();

const geometry = new THREE.PlaneGeometry( 10, 10, 1 );
// const geometry = new THREE.BoxGeometry( 10, 10, 10 );
// const geometry = new THREE.SphereGeometry( 5, 32, 32 );
// Shader example:
const material = new THREE.RawShaderMaterial( {
  uniforms: {
		time: { value: 1.0 }
  },
  vertexShader: `
  precision mediump float;
  precision mediump int;


  uniform mat4 modelViewMatrix; // optional
  uniform mat4 projectionMatrix; // optional
  attribute vec3 position;

  varying vec3 vPosition;
  void main()	{
    vPosition = position;
    gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
  }
  `,
  fragmentShader: `
  precision mediump float;
  precision mediump int;

  varying vec3 vPosition;

  uniform float time;
  void main()	{
    vec2 uv = gl_FragCoord.xy / vec2( 800, 600 );
    gl_FragColor = vec4(uv.x, 0., 0., 1.0);
  }
  `
} );
const plane = new THREE.Mesh( geometry, material );
// plane.position.x = -10;
scene.add( plane );

// Render Loop
var render = function () {
  requestAnimationFrame( render );

  // required if controls.enableDamping or controls.autoRotate are set to true
	controls.update();

  // Render the scene
  renderer.render(scene, camera);
};

render();