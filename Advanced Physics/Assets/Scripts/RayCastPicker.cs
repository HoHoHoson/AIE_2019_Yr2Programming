using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent (typeof(Camera))]
public class RayCastPicker : MonoBehaviour
{
    public float holdDistance = 2.0f;
    public float throwForce = 50.0f;
    public float pullForce = 10.0f;
    public float arrivalRadius = 0.5f;

    private Camera cam = null;
    private RaycastHit hit;
    private Ray ray;
    private Rigidbody objectHit = null;

	// Use this for initialization
	void Start ()
    {
        cam = GetComponent<Camera>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetKeyDown(KeyCode.Mouse0) && !objectHit)
        {
            ray = cam.ScreenPointToRay(Input.mousePosition);

            if (Physics.Raycast(ray, out hit))
            {
                objectHit = hit.transform.GetComponent<Rigidbody>();

                if (objectHit && objectHit.isKinematic)
                    objectHit = null;
            }
        }

        if (objectHit)
        {
            ray = cam.ScreenPointToRay(Input.mousePosition);

            Vector3 displacement = (cam.transform.position + ray.direction * holdDistance) - objectHit.transform.position;
            float sqrMag = displacement.sqrMagnitude;

            float velocityMultiplier = sqrMag / (arrivalRadius * arrivalRadius);
            velocityMultiplier = (velocityMultiplier > 1) ? 1 : velocityMultiplier;

            objectHit.velocity = displacement.normalized * pullForce * velocityMultiplier;
        }

        if (Input.GetKeyUp(KeyCode.Mouse0) && objectHit)
        {
            // List<Rigidbody> rigidbodies = new List<Rigidbody>(objectHit.transform.root.GetComponentsInChildren<Rigidbody>());

            ray = cam.ScreenPointToRay(Input.mousePosition);

            //foreach (Rigidbody r in rigidbodies)
            objectHit.AddForce((ray.direction * throwForce * objectHit.mass), ForceMode.Impulse);

            objectHit = null;
        }
    }
}
