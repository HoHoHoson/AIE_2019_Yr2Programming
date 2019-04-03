using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class Ragdoll : MonoBehaviour
{
    private Animator animator = null;
    private List<Rigidbody> rigidbodies;
    private Vector3 startPos;

	// Use this for initialization
	void Start ()
    {
        animator = GetComponent<Animator>();
        rigidbodies = new List<Rigidbody>(GetComponentsInChildren<Rigidbody>());
        startPos = transform.position;

        foreach (Rigidbody r in rigidbodies)
            r.isKinematic = true;
	}

    public bool RagdollOn
    {
        get { return !animator.enabled; }
        set
        {
            animator.enabled = !value;

            foreach (Rigidbody r in rigidbodies)
                r.isKinematic = !value;
        }
    }

    public void ResetRagdoll()
    {
        transform.position = startPos;
        RagdollOn = false;
    }
}
