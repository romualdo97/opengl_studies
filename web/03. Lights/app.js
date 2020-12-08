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
renderer.setClearColor("#192a56");

// Configure renderer size
renderer.setSize( container.offsetWidth, container.offsetHeight );

// Append Renderer to DOM
container.appendChild( renderer.domElement );

renderer.shadowMap.enabled = true;
renderer.shadowMap.type = THREE.PCFSoftShadowMap;
// renderer.outputEncoding = THREE.sRGBEncoding;

// ------------------------------------------------
// FUN STARTS HERE
// ------------------------------------------------

// Add orbit controls to the scene.
const controls = new OrbitControls( camera, renderer.domElement );
controls.target.set(0, 0, 0);
controls.update();
// controls.minDistance = 150;

// controls.update() must be called after any manual changes to the camera's transform, example.
// camera.position.set( 0, 20, 100 );
// controls.update();

// Create a Cube Mesh with basic material
var geometry = new THREE.SphereGeometry( 5, 32, 32 );
var material = new THREE.MeshPhongMaterial( {color: 0xffff00} );
var sphere = new THREE.Mesh( geometry, material );
sphere.castShadow = true;
sphere.receiveShadow = true;
sphere.position.y = 6;
scene.add( sphere );

// Create a plane to act as floor.
var geometry = new THREE.PlaneGeometry( 100, 100, 1, 1 );
var material = new THREE.MeshPhongMaterial( { color: 0xf5f6fa } );
var floor = new THREE.Mesh( geometry, material );
// Angle is in radians.
floor.rotation.x = Math.PI / 2;
// To see both faces of the mesh.
floor.material.side = THREE.DoubleSide;
// floor.castShadow = true;
floor.receiveShadow = true;
// Add floor to scene.
scene.add( floor ); 

// white spotlight shining from the side, casting a shadow
var spotLight = new THREE.SpotLight( 0xffffff, 1 );
spotLight.position.set( 500, 400, 200 );
spotLight.angle = 0.4;
spotLight.penumbra = 0.05;
spotLight.decay = 1;
spotLight.distance = 2000;
spotLight.castShadow = true;
spotLight.target.position.set( 3, 0, - 3 );
scene.add( spotLight );

// To debug where is the light.
const spotLightHelper = new THREE.SpotLightHelper( spotLight );
scene.add( spotLightHelper );

// Render Loop
var render = function () {
  requestAnimationFrame( render );

  // required if controls.enableDamping or controls.autoRotate are set to true
	controls.update();

  // Render the scene
  renderer.render(scene, camera);
};

render();