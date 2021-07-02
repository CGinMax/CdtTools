import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qaterial 1.0 as Qaterial

import PressPlateTools 1.0
import "../components"

Qaterial.GroupBox {
    id: _root
    title: qsTr("Gather Configuration")
    inlineTitle: true
    Layout.fillWidth: true
    Layout.preferredWidth: 500
    Layout.fillHeight: true

    property bool isStarted: false
    property var portParam: null
    property alias gatherController: controller_gather

    ColumnLayout {

        Qaterial.FlatButton {
            id: btn_run
            text: _root.isStarted ? qsTr("Close") : qsTr("Open")

        }
        Row {
            Qaterial.TextField {
                id: input_address
                title: qsTr("Address")
                text: "1"
                validator: IntValidator{ bottom: 1 }

            }
            LoadingButton {
                id: btn_configure_address
//                opacity: input_address.focus ? 1.0 : 0.0
                iconSource: "image://faicon/arrow-alt-circle-left"
                iconSize: 18
            }

            Qaterial.TextField {
                id: input_sensor_count
                title: qsTr("Sensor Count")
                text: "0"
                validator: IntValidator{ bottom: 0 }

            }
            LoadingButton {
                id: btn_configure_sensor_count
//                opacity: input_sensor_count.focus ? 1.0 : 0.0
                iconSource: "image://faicon/arrow-alt-circle-left"
                iconSize: 18
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Row {
                id: layout_version

                Layout.fillWidth: true
                Layout.preferredWidth: 200
                LabelTile {
                    id: label_hardware
                    title: qsTr("Hardware Version")
                    text: "0.0"


                }
                LabelTile {
                    id: label_software
                    title: qsTr("Software Version")
                    text: "0.0"

                }
                LoadingButton {
                    id: btn_query_version
                    iconSource: "image://faicon/search"
                    iconSize: 18
                }
            }
            LabelTile {
                id: label_product
                title: qsTr("Product Description")
                text: qsTr("None")
                Layout.fillWidth: true
            }
        }
    }
    function alert() {
        Qaterial.SnackbarManager.show({text: qsTr("Communication not open!Can not operate!")})
    }

    function queryVersion() {
        if (!controller_gather.isConnected()) {
            btn_query_version.changeState(false)
            alert()
            return;
        }
        controller_gather.queryGatherVersion(parseInt(input_address.text))
    }

    function configureAddress() {
        if (!controller_gather.isConnected()) {
            btn_configure_address.changeState(false)
            alert()
            return;
        }
        controller_gather.configureGatherAddress(parseInt(input_address.text))

    }

    function configureSensorCount() {
        if (!controller_gather.isConnected()) {
            btn_configure_sensor_count.changeState(false)
            alert()
            return;
        }
        controller_gather.configureSensorCount(parseInt(input_address.text), parseInt(input_sensor_count.text))
    }

    function toggleCommunication() {
        if (controller_gather.isConnected()) {
            controller_gather.stopCommunication()
            return ;
        } else if (_root.portParam === null) {
            console.log("portParam is null")
        } else if (!controller_gather.startCommunication(_root.portParam)) {
            console.log('gather start failed')
        }else {
            _root.isStarted = !_root.isStarted
        }
    }


    GatherController{
        id: controller_gather
        onQueryVersionCallback: function(success, hardware, software, product) {
            if (success) {
                label_hardware.text = hardware
                label_software.text = software
                label_product.text = product

            }

            btn_query_version.changeState(success)
        }
        onConfigureAddressCallback: function(success, addr) {
            if (success) {
                input_address.text = addr
            }
            btn_configure_address.changeState(success)
        }
        onConfigureCountCallback: function(success, count) {
            if (success) {
                input_sensor_count.text = count
            }
            btn_configure_sensor_count.changeState(success)
        }
    } // GatherController

    Component.onCompleted: {
        btn_run.clicked.connect(_root.toggleCommunication)
        btn_configure_address.clicked.connect(_root.configureAddress)
        btn_configure_sensor_count.clickStarted.connect(_root.configureSensorCount)
        btn_query_version.clickStarted.connect(_root.queryVersion)
    }
}
