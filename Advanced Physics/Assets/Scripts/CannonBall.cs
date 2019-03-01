using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof (Rigidbody))]
public class CannonBall : MonoBehaviour
{
    public float forceOnFire = 300;

    bool fire = false;
    bool canFire = true;

    Rigidbody rigidBody = null;

	// Use this for initialization
	void Start ()
    {
		
	}

    private void Awake()
    {
        rigidBody = GetComponent<Rigidbody>();
        rigidBody.isKinematic = true;
    }

    // Update is called once per frame
    void Update ()
    {
		if (Input.anyKeyDown && canFire)
        {
            rigidBody.isKinematic = false;
            rigidBody.AddForce(transform.forward * forceOnFire);
            canFire = false;
        }
	}
}
