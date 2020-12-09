// Find the latest version by visiting https://unpkg.com/three. The URL will
// redirect to the newest stable release.
import * as THREE from 'https://unpkg.com/three@0.123.0/build/three.module.js';
// Only a few loaders (e.g. ObjectLoader) are included by default with three.js — others should be added to your app individually.
import { GLTFLoader } from 'https://unpkg.com/three@0.123.0/examples/jsm/loaders/GLTFLoader.js';
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

// Modelo extracted from:
// https://sketchfab.com/3d-models/the-mill-485266e0d43e46a7bc7850d3d957571a
// Tutorial about loading models here:
// https://threejs.org/docs/#manual/en/introduction/Loading-3D-models

// Instantiate the GLTF loader
var loader = new GLTFLoader();

// If you load a model and it's black try using these visualizers to check
// the models are right:
// 1. https://gltf-viewer.donmccurdy.com/
// 2. https://sandbox.babylonjs.com/
// If model is too dark try adding a light to the scene and incresing the intensity.

// Load a glTF resource
loader.load(
  // resource URL
  'models/the_mill/scene.gltf',
  // called when the resource is loaded
  function ( gltf ) {
    // Add the model internal scene to the three.js scene.
    scene.add( gltf.scene );

    // console.log( gltf.scene );
    console.log( gltf );
    // gltf.animations; // Array<THREE.AnimationClip>
		// gltf.scene; // THREE.Group
		// gltf.scenes; // Array<THREE.Group>
		// gltf.cameras; // Array<THREE.Camera>
		// gltf.asset; // Object
  },
  // called while loading is progressing
  function ( xhr ) {
		console.log( ( xhr.loaded / xhr.total * 100 ) + '% loaded' );
	},
  // called when loading has errors
	function ( error ) {
    console.log( 'An error happened' );
    console.log( error );
	}
);



// Add orbit controls to the scene.
const controls = new OrbitControls( camera, renderer.domElement );
controls.minDistance = 150;

// controls.update() must be called after any manual changes to the camera's transform, example.
// camera.position.set( 0, 20, 100 );
// controls.update();

// Render Loop
var render = function () {
  requestAnimationFrame( render );

  // required if controls.enableDamping or controls.autoRotate are set to true
	controls.update();

  // Render the scene
  renderer.render(scene, camera);
};

render();