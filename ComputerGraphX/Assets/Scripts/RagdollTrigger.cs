using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Any object with this script will ragdoll objects with the ragdoll class, on collision
/// </summary>
[RequireComponent(typeof(Collider))]
public class RagdollTrigger : MonoBehaviour
{
    /// <summary>
    /// Triggers the <paramref name="other"/> object's collider, onTrigger
    /// </summary>
    /// <param name="other"></param>
    private void OnTriggerEnter(Collider other)
    {
        ActivateRagdoll(other.gameObject);
    }

    /// <summary>
    /// Triggers the <paramref name="collision"/> object's ragdoll, onCollision
    /// </summary>
    /// <param name="collision"></param>
    private void OnCollisionEnter(Collision collision)
    {
        ActivateRagdoll(collision.gameObject);
    }

    /// <summary>
    /// Triggers the <paramref name="GO"/>'s Ragdoll class to ragdoll
    /// </summary>
    /// <param name="GO"></param>
    private void ActivateRagdoll(GameObject GO)
    {
        Ragdoll ragdoll = GO.GetComponentInParent<Ragdoll>();

        if (ragdoll != null)
            ragdoll.RagdollOn = true;
    }
}
