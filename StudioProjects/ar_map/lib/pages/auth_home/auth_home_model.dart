import '/flutter_flow/flutter_flow_util.dart';
import 'auth_home_widget.dart' show AuthHomeWidget;
import 'package:flutter/material.dart';

class AuthHomeModel extends FlutterFlowModel<AuthHomeWidget> {
  ///  State fields for stateful widgets in this page.

  final unfocusNode = FocusNode();

  @override
  void initState(BuildContext context) {}

  @override
  void dispose() {
    unfocusNode.dispose();
  }
}
