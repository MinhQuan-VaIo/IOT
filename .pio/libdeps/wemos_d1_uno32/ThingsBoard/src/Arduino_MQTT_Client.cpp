// Header include.
#include "Arduino_MQTT_Client.h"

#ifdef ARDUINO

Arduino_MQTT_Client::Arduino_MQTT_Client(Client & transport_client) :
    m_cb(nullptr),
    m_mqtt_client(transport_client)
{
    // Nothing to do
}

void Arduino_MQTT_Client::set_client(Client & transport_client) {
    m_mqtt_client.setClient(transport_client);
}

void Arduino_MQTT_Client::set_data_callback(data_function cb) {
    m_mqtt_client.setCallback(cb);
}

void Arduino_MQTT_Client::set_connect_callback(connect_function cb) {
    m_cb = cb;
}

bool Arduino_MQTT_Client::set_buffer_size(uint16_t const & buffer_size) {
    return m_mqtt_client.setBufferSize(buffer_size);
}

uint16_t Arduino_MQTT_Client::get_buffer_size() {
    return m_mqtt_client.getBufferSize();
}

void Arduino_MQTT_Client::set_server(char const * const domain, uint16_t const & port) {
    m_mqtt_client.setServer(domain, port);
}

bool Arduino_MQTT_Client::connect(char const * const client_id, char const * const user_name, char const * const password) {
    bool const result = m_mqtt_client.connect(client_id, user_name, password);
    m_cb();
    return result;
}

void Arduino_MQTT_Client::disconnect() {
    m_mqtt_client.disconnect();
}

bool Arduino_MQTT_Client::loop() {
    return m_mqtt_client.loop();
}

bool Arduino_MQTT_Client::publish(char const * const topic, uint8_t const * const payload, size_t const & length) {
    return m_mqtt_client.publish(topic, payload, length, false);
}

bool Arduino_MQTT_Client::subscribe(char const * const topic) {
    return m_mqtt_client.subscribe(topic);
}

bool Arduino_MQTT_Client::unsubscribe(char const * const topic) {
    return m_mqtt_client.unsubscribe(topic);
}

bool Arduino_MQTT_Client::connected() {
    return m_mqtt_client.connected();
}

#if THINGSBOARD_ENABLE_STREAM_UTILS

bool Arduino_MQTT_Client::begin_publish(char const * const topic, size_t const & length) {
    return m_mqtt_client.beginPublish(topic, length, false);
}

bool Arduino_MQTT_Client::end_publish() {
    return m_mqtt_client.endPublish();
}

size_t Arduino_MQTT_Client::write(uint8_t payload_byte) {
    return m_mqtt_client.write(payload_byte);
}

size_t Arduino_MQTT_Client::write(uint8_t const * const buffer, size_t const & size) {
    return m_mqtt_client.write(buffer, size);
}

#endif // THINGSBOARD_ENABLE_STREAM_UTILS

#endif // ARDUINO
