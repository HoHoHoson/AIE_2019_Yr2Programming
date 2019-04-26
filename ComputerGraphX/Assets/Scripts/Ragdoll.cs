using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Class for enabling and disabling Unity ragdolls. Interacts with the RagdollTrigger class
/// </summary>
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

    /// <summary>
    /// Enables/Disables the ragdoll by turning the object's animator On/Off and setting it's Rigidbodies isKinematic to False/True
    /// </summary>
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

    /// <summary>
    /// Returns the ragdoll to its initial position from when it was loaded in and turns ragdoll off.
    /// </summary>
    public void ResetRagdoll()
    {
        transform.position = startPos;
        RagdollOn = false;
    }
}
