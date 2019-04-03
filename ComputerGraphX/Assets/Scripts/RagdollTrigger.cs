using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Collider))]
public class RagdollTrigger : MonoBehaviour
{
    private void OnTriggerEnter(Collider other)
    {
        ActivateRagdoll(other.gameObject);
    }

    private void OnCollisionEnter(Collision collision)
    {
        ActivateRagdoll(collision.gameObject);
    }

    private void ActivateRagdoll(GameObject GO)
    {
        Ragdoll ragdoll = GO.GetComponentInParent<Ragdoll>();

        if (ragdoll != null)
            ragdoll.RagdollOn = true;
    }
}
