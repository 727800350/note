# pulsar
## introduction
[Introduction to the Apache Pulsar pub-sub messaging platform](https://streaml.io/blog/intro-to-pulsar)

Pulsar was built from the ground up to support multi-tenant use cases.
Pulsar supports two multi-tenancy-specific resources to enable multi-tenancy: properties and namespaces.
A property represents a tenant in the system. To give an example, imagine a Pulsar cluster deployed to support a wide variety of applications (as was the case with Pulsar at Yahoo).
Within a Pulsar cluster, each property can represent a team in the enterprise, a core feature, or a product line, to give just a few examples.
Each property, in turns, can contain several namespaces, for example one namespace for each application or use case.
A namespace can then contain any number of topics.

<img src="./pics/pulsar/hierarchy.png" alt="pulsar hierarchy" width="60%"/>

The namespace is the basic administrative unit in Pulsar.
At the namespace level, you can set permissions, fine-tune replication settings, manage geo-replication of message data across clusters, control message expiry, and perform critical operations.
All topics in a namespace inherit the same settings,

As mentioned before, each namespace can have one or more topics;
each topic can have multiple subscriptions; and each subscription is set to retain and receive all messages published on the topic.
To provide even more flexibility to application, Pulsar enables three different types of subscriptions that can coexist on the same topic:

- Exclusive subscription: there can be only a single consumer at any given time.
- Shared subscription: multiple consumers can attach to the same subscription and each consumer will receive a fraction of the messages.
- Failover subscription: multiple consumers are allowed to connect to a topic but only one consumer will receive messages at any given time.
  The other consumers will start receiving messages only when the current receeiving consumer fails.

<img src="./pics/pulsar/subscription_types.png" alt="subscription_types" width="60%"/>

