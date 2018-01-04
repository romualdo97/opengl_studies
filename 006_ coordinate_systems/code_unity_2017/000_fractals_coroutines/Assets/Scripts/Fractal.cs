// read tuto here:
// http://catlikecoding.com/unity/tutorials/constructing-a-fractal/

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Fractal : MonoBehaviour {

    public int maxDepth;
    int depth;
    public float childScale;
    public Mesh mesh;
    public Material material;

	// Use this for initialization
	void Start ()
    {
        gameObject.AddComponent<MeshFilter>().mesh = mesh;
        gameObject.AddComponent<MeshRenderer>().material = material;
        if (depth < maxDepth)
        {
            StartCoroutine(CreateChildren());
        }
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    private IEnumerator CreateChildren()
    {
        yield return new WaitForSeconds(0.5f);
        new GameObject("FractalChild").AddComponent<Fractal>().Initialize(this, Vector3.up, Quaternion.identity);
        yield return new WaitForSeconds(0.5f);
        new GameObject("FractalChild").AddComponent<Fractal>().Initialize(this, Vector3.right, Quaternion.Euler(0f, 0f, -90f));
        yield return new WaitForSeconds(0.5f);
        new GameObject("FractalChild").AddComponent<Fractal>().Initialize(this, Vector3.left, Quaternion.Euler(0f, 0f, 90f));
    }

    private void Initialize(Fractal parent, Vector3 direction, Quaternion orientation)
    {
        mesh = parent.mesh;
        material = parent.material;
        maxDepth = parent.maxDepth;
        depth = parent.depth + 1;
        childScale = parent.childScale;
        transform.parent = parent.transform;
        transform.localScale = Vector3.one * childScale;
        transform.localPosition = direction * (0.5f + 0.5f * childScale);
        transform.localRotation = orientation;
    }
}
