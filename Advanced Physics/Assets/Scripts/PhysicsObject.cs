using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class PhysicsObject : MonoBehaviour
{
    public Material awakeMat = null;
    public Material sleepMat = null;

    private Rigidbody _rigidbody = null;

    bool wasSleeping = false;

	// Use this for initialization
	void Start ()
    {
        _rigidbody = GetComponent<Rigidbody>();
	}
	
	// Update is called once per frame
	void Update ()
    {

	}

    private void FixedUpdate()
    {
        if (_rigidbody.IsSleeping() && !wasSleeping && sleepMat != null)
        {
            wasSleeping = true;
            GetComponent<MeshRenderer>().material = sleepMat;
        }

        if (!_rigidbody.IsSleeping() && wasSleeping && awakeMat != null)
        {
            wasSleeping = false;
            GetComponent<MeshRenderer>().material = awakeMat;
        }
    }
}
